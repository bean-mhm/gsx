#pragma once

// STD
#include <string>
#include <set>
#include <cstdint>

// Internal
#include "event.h"
#include "../misc.h"

namespace tef::ecs
{

    // Forward declerations
    class world_t;
    struct world_iteration_t;

    // Abstract class for a system
    // Note: The abstract functions will be called at appropriate times by the parent world while
    // it is running.
    class base_system_t
    {
    public:
        // A name for the system
        const std::string name;

        // Update order. A system with a higher order value will have its on_update() function
        // invoked after a system with a lower order value. Systems with the same order will have
        // their on_update() functions invoked in parallel.
        // Note: Avoid modifying this value when the parent world is running.
        int32_t update_order = 0;

        // Force the parent world to invoke the abstract functions of this system using the same
        // thread that is running the world. If multiple systems with identical update orders have
        // this option enabled, they will not be parallelized, but the other systems at their
        // order will be updated together in parallel. This option is especially helpful when
        // working with single-threaded contexts like in the OpenGL API.
        bool run_on_caller_thread = false;

        // Which event types will trigger this system?
        std::set<event_type_t> triggers;

        // Create a system with given properties.
        base_system_t(const std::string& name, int32_t update_order, bool run_on_caller_thread);
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

}
