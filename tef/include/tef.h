#pragma once

#include <string>
#include <vector>
#include <set>
#include <deque>
#include <unordered_map>
#include <any>
#include <mutex>
#include <memory>
#include <cstdint>

#include "tef_prng.h"
#include "tef_utils.h"

namespace tef
{

    using byte_t = uint8_t;
    using id_t = uint64_t;
    using tag_t = uint64_t;
    using event_type_t = uint64_t;
    using entity_t = id_t;

    class world_t;
    struct world_iteration_t;

    struct event_t
    {
        event_type_t type;
        std::any data;

        event_t(event_type_t type, const std::any& data);
    };

    // Abstract struct for a component. Subclasses should not allocate heap memory. A component
    // has no functionality, it only ever stores data.
    struct component_t
    {
        entity_t owner;
    };

    // Abstract struct for a system. A system generally should not have a state.
    struct system_t
    {
        const std::string name;

        // Which event types will trigger this system?
        std::set<event_type_t> triggers;

        system_t(const std::string& name);
        no_default_copy_move_constructor(system_t);
        virtual ~system_t();

        // Called when the world starts running
        virtual void on_start(world_t& world);

        // Called every iteration
        virtual void on_update(world_t& world, const world_iteration_t& iter);

        // Called when triggered by an event
        virtual void on_trigger(world_t& world, const world_iteration_t& iter, const event_t& event);

        // Called when the world stops running
        virtual void on_stop(world_t& world, const world_iteration_t& iter);
    };

    // A world for holding and managing a collection of entities, components, and systems.
    class world_t
    {
    public:
        const std::string name;
        std::mutex mutex;
        prng_t prng;

        world_t(const std::string& name);
        no_default_copy_move_constructor(world_t);

        entity_t add_entity();
        void remove_entity(entity_t entity);

        // T must be derived from component_t.
        template <typename T>
        void get_components_of_type(T*& out_components, size_t& out_size)
        {
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();
            out_components = (T*)(bytes.data());
            out_size = bytes.size() / sizeof(T);
        }

        // T must be derived from component_t.
        // Do not add the same component to the same entity twice.
        template <typename T>
        void add_component(const T& initial_values)
        {
            // Component list for T
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();

            // Make space
            size_t old_size = bytes.size();
            bytes.resize(bytes.size() + sizeof(T));

            // Add component with the given values
            std::copy((byte_t*)(&initial_values), (byte_t*)(&initial_values) + sizeof(T), bytes.data() + old_size);
        }

        // T must be derived from component_t.
        template <typename T>
        void remove_component(entity_t owner)
        {
            // Component list for T
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();

            // Find the index of entity
            ptrdiff_t offset = find_entity_index_for_component_bytes<T>(owner);

            // No component of type T is attached to the entity
            if (offset < 0)
                return;

            // Erase
            bytes.erase(bytes.begin() + offset, bytes.begin() + offset + sizeof(T));
        }

        void remove_components_owned_by(entity_t owner);

        // T must be derived from component_t.
        template <typename T>
        T* get_component_of_type_owned_by(entity_t owner)
        {
            ptrdiff_t offset = find_entity_index_for_component_bytes<T>(owner);
            if (offset < 0)
            {
                return nullptr;
            }
            else
            {
                std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();
                return (T*)(&bytes[offset]);
            }
        }

        // Duplication of a system type may result in undefined behavior.
        void add_system(std::shared_ptr<system_t> system);

        // Remove the first system in the list with a given name.
        void remove_system(const std::string& name);

        // Enqueue a given event
        void enqueue_event(const event_t& event);

        // Start the main loop. Only a single thread can be running the main loop at a time.
        void run(float max_update_rate = 0);

        // Signal the runner thread to stop, and optionally wait for it.
        void stop(bool wait);

    private:
        struct component_list_t
        {
            size_t stride = 0;
            std::vector<byte_t> data;
        };

        std::deque<event_t> events;
        std::vector<entity_t> entities;

        // Maps a component type to a list of components of that type
        std::unordered_map<size_t, component_list_t> comp_map;

        std::vector<std::shared_ptr<system_t>> systems;

        bool running = false;
        bool should_stop = false;

        // Byte array for components of type T
        template <typename T>
        std::vector<byte_t>& get_components_of_type_bytes()
        {
            // Type ID hash
            const size_t type_hash = typeid(T).hash_code();

            // See if we've already made a list for this type
            if (comp_map.contains(type_hash))
            {
                return comp_map[type_hash].data;
            }
            else
            {
                comp_map[type_hash] = component_list_t();
                comp_map[type_hash].stride = sizeof(T);
                return comp_map[type_hash].data;
            }
        }

        // Find the index (in bytes) of the first component of type T owned by a given entity
        template <typename T>
        ptrdiff_t find_entity_index_for_component_bytes(entity_t entity)
        {
            std::vector<byte_t>& bytes = get_components_of_type_bytes<T>();
            for (size_t i = 0; i < bytes.size(); i += sizeof(T))
            {
                T* elem = (T*)(&bytes[i]);
                if (elem->owner == entity)
                {
                    return i;
                }
            }
            return -1;
        }

    };

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
