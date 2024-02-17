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

    // information on how a system must be run
    struct execution_scheme_t
    {
        // a system with a higher update order will have its on_update()
        // function invoked after a system with a lower update order. systems
        // with the same order will have their on_update() functions invoked in
        // parallel.
        // * the on_start() functions are invoked in the same order in which the
        //   systems were added to the world, while the on_stop() functions are
        //   invoked in reverse order.
        i32 update_order = 0;

        // force the parent world to invoke the abstract functions of the system
        // on the same thread that is running the world. if this option is
        // disabled, the system may or may not run in a separate thread. if it
        // does so, all of its abstract functions will be invoked on that thread
        // only. otherwise, they will be invoked on the same thread that is
        // running the world. this option is especially helpful when working
        // with single-threaded contexts like in the OpenGL API.
        // * if several systems with the same update order have this option
        //   enabled, they will not be parallelized, as they'll need to run on the
        //   same thread.
        bool run_on_world_thread = false;

        constexpr execution_scheme_t(
            i32 update_order,
            bool run_on_world_thread = false
        )
            : update_order(update_order),
            run_on_world_thread(run_on_world_thread)
        {}
    };

    // * the abstract functions will be called at appropriate times by the
    //   parent world while it is running.
    class base_system_t
    {
    public:
        const std::string name;
        const execution_scheme_t exec_scheme;
        std::set<event_type_t> triggers;

        base_system_t(
            const std::string& name,
            const execution_scheme_t& exec_scheme
        );
        no_copy_construct_no_assignment(base_system_t);
        virtual ~base_system_t() = default;

        // called when the world starts running, in the order in which the
        // systems were added.
        // * avoid starting separate threads that keep running after returning
        //   from this function.
        virtual void on_start(world_t& world);

        // called when triggered by an event.
        // * avoid starting separate threads that keep running after returning
        //   from this function.
        virtual void on_trigger(
            world_t& world,
            const iteration_t& iter,
            const event_t& event
        );

        // called in every iteration when the world is running. a system would
        // typically get a list of components it's interested in, iterate over
        // them, and update them. the iteration can be manually parallelized by
        // the user for improved performance.
        // * avoid starting separate threads that keep running after returning
        //   from this function.
        virtual void on_update(world_t& world, const iteration_t& iter);

        // called when the world stops running, in the opposite order to that in
        // which the systems were added.
        // * avoid starting separate threads that keep running after returning
        //   from this function.
        virtual void on_stop(world_t& world, const iteration_t& iter);

    };

}
