#pragma once

// STD
#include <string>
#include <vector>
#include <set>
#include <deque>
#include <unordered_map>
#include <any>
#include <mutex>
#include <memory>
#include <typeinfo>
#include <typeindex>
#include <functional>
#include <thread>
#include <cstdint>

// Internal
#include "tef_log.h"
#include "tef_prng.h"
#include "tef_utils.h"

namespace tef
{

    // Type definitions
    using byte_t = uint8_t;
    using entity_t = uint64_t;
    using event_type_t = uint64_t;

    // Forward declerations
    class world_t;
    struct world_iteration_t;

    // Event
    struct event_t
    {
        // Event type specified by the sender
        event_type_t type;

        // Event data
        std::any data;

        event_t(event_type_t type, const std::any& data);
    };

    // Abstract struct for a component
    // Note: A component should never allocate heap memory. For example, don't use std::vector.
    // Note: A component has no functionality, it only ever stores data.
    // Note: Component types don't necessarily have to be derived from base_component_t, but look
    // out for templated functions that specifically mention the need for T to be derived from
    // base_component_t.
    struct base_component_t
    {
        entity_t owner;
    };

    // Abstract struct for a system
    // Note: The abstract functions will be called at appropriate times by the parent world while
    // it is running.
    struct base_system_t
    {
        // A unique name for the system
        const std::string name;

        // Update order. A system with a higher order value will have its on_update() function
        // invoked after a system with a lower order value. Systems with the same order will have
        // their on_update() functions invoked in parallel.
        // Note: Avoid modifying this value when the parent world is running.
        int32_t update_order = 0;

        // Force the parent world to invoke the abstract functions of this system using the same
        // thread that is running the world. If multiple systems with identical update orders have
        // this option enabled, they will not be parallelized, but the other systems at their
        // order will be updated together in parallel.
        bool run_on_caller_thread = false;

        // Which event types will trigger this system?
        std::set<event_type_t> triggers;

        // Create a system with a given name.
        base_system_t(const std::string& name, int32_t update_order);
        no_default_copy_move_constructor(base_system_t);
        virtual ~base_system_t();

        // Called when the world starts running, in the order in which the systems were added.
        // Note: Avoid starting separate threads that keep running after returning from this
        // function.
        virtual void on_start(world_t& world);

        // Called in every iteration when the world is running. A system would typically get a
        // list of components it's interested in, iterate over them, and update them. The
        // iteration can be manually parallelized by the user for improved performance.
        // Note: Avoid starting separate threads that keep running after returning from this
        // function.
        virtual void on_update(world_t& world, const world_iteration_t& iter);

        // Called when triggered by an event.
        // Note: Avoid starting separate threads that keep running after returning from this
        // function.
        virtual void on_trigger(world_t& world, const world_iteration_t& iter,
            const event_t& event);

        // Called when the world stops running, in the opposite order to that in which the systems
        // were added.
        // Note: Avoid starting separate threads that keep running after returning from this
        // function.
        virtual void on_stop(world_t& world, const world_iteration_t& iter);
    };

    // A world for holding and managing a collection of components and systems
    class world_t
    {
    public:
        // A name for the world
        const std::string name;

        // Maximum log level to use
        const log_level_t max_log_level;

        // Mutex for the components
        // Note: Not used internally, can optionally be used by the developer for synchronization.
        // You might want to use this when adding/removing components using several threads for
        // example.
        std::mutex mutex_components;

        // Mutex for the systems
        // Note: Not used internally, can optionally be used by the developer for synchronization.
        // You might want to use this when adding/removing systems using several threads for
        // example.
        std::mutex mutex_systems;

        // A PRNG (Pseudo-Random Number Generator) for the world
        prng_t prng;

        // Create a world with a given name and a logger.
        world_t(
            const std::string& name,
            log_level_t max_log_level,
            std::shared_ptr<base_logger_t> logger
        );
        no_default_copy_move_constructor(world_t);
        ~world_t();

        // Add a custom log message.
        void log(log_level_t log_level, const std::string& message);

        // Enqueue a given event.
        void enqueue_event(const event_t& event);

        // Get the first component of type T owned by a given entity. If no such component exists,
        // nullptr will be returned.
        // Note: T must be derived from base_component_t.
        template <typename T>
        T* get_component_of_type_owned_by(entity_t owner)
        {
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();
            for (size_t i = 0; i < bytes.size(); i += sizeof(T))
            {
                T* elem = (T*)(&bytes[i]);
                if (elem->owner == owner)
                {
                    return elem;
                }
            }
            return nullptr;
        }

        // Get a list of all components of type T.
        template <typename T>
        void get_components_of_type(T*& out_components, size_t& out_size)
        {
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();
            out_components = (T*)(bytes.data());
            out_size = bytes.size() / sizeof(T);
        }

        // Add a component of type T with given initial values. You cannot use initial_values to
        // modify the component afterward. Instead, a pointer to the new copy in the component
        // list will be returned.
        template <typename T>
        T* add_component_of_type(const T& initial_values)
        {
            tef_log(this, log_level_t::verbose, utils::str_format(
                "Adding a new component of type \"%s\"",
                typeid(T).name()
            ));

            // Component list for T
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();

            // Make space
            size_t old_size = bytes.size();
            bytes.resize(bytes.size() + sizeof(T));

            // Add component with given values
            std::copy(
                (byte_t*)(&initial_values),
                (byte_t*)(&initial_values) + sizeof(T),
                bytes.data() + old_size
            );

            // Return a pointer to the new copy
            return (T*)(bytes.data() + old_size);
        }

        // Remove the first component of type T owned by a given entity.
        // Note: T must be derived from base_component_t.
        template <typename T>
        void remove_component_of_type_owned_by(entity_t owner)
        {
            tef_log(this, log_level_t::verbose, utils::str_format(
                "Removing the first component of type \"%s\" owned by %s",
                typeid(T).name(),
                std::to_string(owner).c_str()
            ));

            // Component list for T
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();

            // Remove the first element owned by the owner
            for (size_t i = 0; i < bytes.size(); i += sizeof(T))
            {
                T* elem = (T*)(&bytes[i]);
                if (elem->owner == owner)
                {
                    bytes.erase(bytes.begin() + i, bytes.begin() + i + sizeof(T));
                    break;
                }
            }
        }

        // Remove all components of type T owned by a given entity.
        // Note: T must be derived from base_component_t.
        template <typename T>
        void remove_components_of_type_owned_by(entity_t owner)
        {
            tef_log(this, log_level_t::verbose, utils::str_format(
                "Removing all components of type \"%s\" owned by %s",
                typeid(T).name(),
                std::to_string(owner).c_str()
            ));

            // Component list for T
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();

            // Iterate over the components
            for (size_t i = 0; i < bytes.size();)
            {
                // Check the owner
                T* elem = (T*)(&bytes[i]);
                if (elem->owner == owner)
                {
                    // Erase
                    bytes.erase(bytes.begin() + i, bytes.begin() + i + sizeof(T));
                }
                else
                {
                    // Continue iterating
                    i += sizeof(T);
                }
            }
        }

        // Remove all components of all types owned by a given entity.
        // Note: T must be derived from base_component_t.
        void remove_components_owned_by(entity_t owner);

        // Remove all components of type T.
        template <typename T>
        void remove_components_of_type()
        {
            tef_log(this, log_level_t::verbose, utils::str_format(
                "Removing all components of type \"%s\"",
                typeid(T).name()
            ));

            // Type index
            const std::type_index ti = std::type_index(typeid(T));

            // See if there's a list for this type
            if (comp_map.contains(ti))
            {
                comp_map.erase(ti);
            }
        }

        // Remove all components in the world.
        void remove_components();

        // Get the first system in the list with a given name. If no such system exists, nullptr
        // will be returned.
        std::shared_ptr<base_system_t> get_system_named(const std::string& name);

        // Add a system to the world.
        void add_system(const std::shared_ptr<base_system_t>& system);

        // Remove the first system in the list with a given name.
        void remove_system_named(const std::string& name);

        // Remove all systems with a given name.
        void remove_systems_named(const std::string& name);

        // Remove all systems in the world.
        void remove_systems();

        // Start the main loop with a given maximum update rate. This will call the abstract
        // functions of the systems present in the world.
        // Note: Avoid adding or removing systems while the world is running, as it will have no
        // effect.
        // Note: Only a single thread can be running the world at a time.
        // Note: Use a max_update_rate of 0 for uncapped update rate.
        // Note: Use a max_run_time of 0 for uncapped run time.
        void run(const float max_update_rate = 0, const float max_run_time = 0);

        // Signal the runner thread to stop, and optionally wait for it if calling from a separate
        // thread. If this is called from the same thread that called run(), wait must be false.
        void stop(bool wait);

    private:
        // Logger
        std::shared_ptr<base_logger_t> logger;

        // Internal mutex for running the world
        std::mutex mutex_run;

        // Internal mutex for the event queue
        std::mutex mutex_events;

        // Wrapper for a byte array holding a list of components of the same type
        struct component_list_t
        {
            // How many bytes does a single component take up in memory?
            size_t stride = 0;

            // Byte array
            std::vector<byte_t> data;
        };

        // Event queue
        std::deque<event_t> events;

        // Mapping from a component type to a list of components of that type
        std::unordered_map<std::type_index, component_list_t> comp_map;

        // Systems
        std::vector<std::shared_ptr<base_system_t>> systems;

        // Should the world stop running?
        bool should_stop = false;

        // Get a byte array of all components of type T.
        template <typename T>
        std::vector<byte_t>& get_components_of_type_bytes()
        {
            // Type index
            const std::type_index ti = std::type_index(typeid(T));

            // If there's no list for this type, create one
            if (!comp_map.contains(ti))
            {
                tef_log(this, log_level_t::verbose, utils::str_format(
                    "Creating a new component list for type \"%s\"",
                    typeid(T).name()
                ));

                comp_map[ti] = component_list_t();
                comp_map[ti].stride = sizeof(T);
            }

            // Return
            return comp_map[ti].data;
        }

    };

    // Information about the current iteration of the world. This will be passed to systems when
    // the world is running.
    struct world_iteration_t
    {
        // Iteration number starting from 0
        uint64_t i = 0;

        // Seconds elapsed since the start
        float elapsed = 0;

        // Seconds elapsed since the last iteration
        float dt = 0;
    };

}
