#pragma once

// STD
#include <string>
#include <set>
#include <cstdint>

// Internal
#include "event.h"
#include "world.h"
#include "../internal_common/all.h"

namespace gsx::ecs
{

    // Information on how a system must be run
    struct execution_scheme_t
    {
        // Update order. A system with a higher order value will have its on_update() function
        // invoked after a system with a lower order value. Systems with the same order will have
        // their on_update() functions invoked in parallel.
        // Note: The on_start() functions are invoked in the same order in which the systems were
        // added to the world, while the on_stop() functions are invoked in reverse.
        i32 update_order = 0;

        // Force the parent world to invoke the abstract functions of a system on the same thread
        // that is running the world. If this option is disabled, the system may or may not run in
        // a separate thread. If it does so, all of its abstract functions will be invoked on that
        // thread only. Otherwise, they will be invoked on the same thread that is running the
        // world. This option is especially helpful when working with single-threaded contexts
        // like in the OpenGL API.
        // Note: If several systems with the same update order values have this option enabled,
        // they will not be parallelized, as they all need to run on the same thread.
        bool run_on_world_thread = false;

        // Constructor
        execution_scheme_t(i32 update_order, bool run_on_world_thread = false);
    };

    // Abstract class for a system
    // Note: The abstract functions will be called at appropriate times by the parent world while
    // it is running.
    class base_system_t
    {
    public:
        // A name for the system
        const std::string name;

        // Execution scheme
        const execution_scheme_t exec_scheme;

        // Which event types will trigger this system?
        std::set<event_type_t> triggers;

        // Create a system with given properties.
        base_system_t(const std::string& name, const execution_scheme_t& exec_scheme);
        no_default_copy_construct_no_assignment(base_system_t);
        virtual ~base_system_t();

        // Called when the world starts running, in the order in which the systems were added.
        // Note: Avoid starting separate threads that keep running after returning from this
        // function.
        virtual void on_start(world_t& world);

        // Called when triggered by an event.
        // Note: Avoid starting separate threads that keep running after returning from this
        // function.
        virtual void on_trigger(world_t& world, const iteration_t& iter, const event_t& event);

        // Called in every iteration when the world is running. A system would typically get a
        // list of components it's interested in, iterate over them, and update them. The
        // iteration can be manually parallelized by the user for improved performance.
        // Note: Avoid starting separate threads that keep running after returning from this
        // function.
        virtual void on_update(world_t& world, const iteration_t& iter);

        // Called when the world stops running, in the opposite order to that in which the systems
        // were added.
        // Note: Avoid starting separate threads that keep running after returning from this
        // function.
        virtual void on_stop(world_t& world, const iteration_t& iter);

    };

}
