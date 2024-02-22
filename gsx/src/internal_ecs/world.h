#pragma once

#include <string>
#include <deque>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <cstdint>

#include "log.h"
#include "event.h"
#include "../internal_common/all.h"
#include "../internal_misc/all.h"

namespace gsx::ecs
{

    class base_system_t;

    // information about the current iteration of the world. this will be passed
    // to systems when the world is running.
    struct iteration_t
    {
        // iteration number starting from 0
        u64 i = 0;

        // seconds elapsed since the start
        f64 time = 0;

        // seconds elapsed since the last iteration
        f64 dt = 0;
    };

    // a world for holding and managing a collection of systems
    class world_t
    {
    public:
        const std::string name;
        const log_level_t max_log_level;

        world_t(
            const std::string& name,
            log_level_t max_log_level,
            std::shared_ptr<base_logger_t> logger
        );
        no_copy_construct_no_assignment(world_t);
        ~world_t();

        void log(log_level_t log_level, const std::string& message);
        void enqueue_event(const event_t& event);

        // get the first system in the list with a given name. if no such
        // system exists, nullptr will be returned.
        std::shared_ptr<base_system_t> get_system_named(
            const std::string& name
        );

        constexpr const std::vector<std::shared_ptr<base_system_t>>&
            get_systems() const
        {
            return systems;
        }

        constexpr std::vector<std::shared_ptr<base_system_t>>& get_systems()
        {
            return systems;
        }

        void add_system(const std::shared_ptr<base_system_t>& system);
        void remove_first_system_named(const std::string& name);
        void remove_all_systems_named(const std::string& name);
        void remove_all_systems();

        // start the main loop with a given maximum update rate. this will call
        // the abstract functions of the systems present in the world.
        // * avoid adding or removing systems while the world is running, as it
        //   will not affect the current run.
        // * only a single thread can be running the world at a time.
        // * use a max_update_rate of 0 for uncapped update rate.
        // * use a max_run_time of 0 for uncapped run time.
        void run(const f64 max_update_rate = 0, const f64 max_run_time = 0);

        // signal the runner thread to stop, and optionally wait for it if
        // calling from a separate thread. if this is called from the same
        // thread that called run(), wait must be false.
        void stop(bool wait);

    private:
        // a group of systems with identical update order values, all to be
        // updated in parallel (except ones with run_on_world_thread=true).
        // here's how the systems could be updated in a hypothetical world:
        // 1. update the movement system, the jump system, and the physics
        //    system, all in parallel.
        // 2. after step 1 is finished, update the collision system.
        // 3. update the audio system and the render system in parallel.
        // think of each step as a system group. notice how the system groups
        // are run in serial, but the systems inside each group are updated
        // together in parallel.
        struct system_group_t
        {
            i32 update_order;
            std::vector<std::shared_ptr<base_system_t>> systems;
        };

        // mapping from a system pointer to a worker thread used to invoke the
        // abtract functions of that system (start, update, trigger, stop). if
        // the worker is nullptr, then the system must run on the same thread
        // that is running the world.
        using worker_map_t =
            std::unordered_map<base_system_t*, std::shared_ptr<misc::worker_t>>;

        std::shared_ptr<base_logger_t> logger;
        std::mutex mutex_run;
        std::mutex mutex_events;
        std::deque<event_t> events;
        std::vector<std::shared_ptr<base_system_t>> systems;
        bool should_stop = false;

        // * this function is called internally by run().
        void prepare_system_groups_and_workers(
            std::vector<std::shared_ptr<base_system_t>>& systems_copy,
            std::vector<system_group_t>& out_system_groups,
            worker_map_t& out_worker_map
        );

        void start_systems(
            std::vector<std::shared_ptr<base_system_t>>& systems_copy,
            worker_map_t& worker_map,
            bool& out_did_start_all
        );

        void process_events(
            std::vector<std::shared_ptr<base_system_t>>& systems_copy,
            worker_map_t& worker_map,
            const iteration_t& iter,
            bool& out_did_process_all
        );

        void update_systems(
            std::vector<system_group_t>& system_groups,
            worker_map_t& worker_map,
            const iteration_t& iter,
            bool& out_did_update_all
        );

        void stop_systems(
            std::vector<std::shared_ptr<base_system_t>>& systems_copy,
            worker_map_t& worker_map,
            const iteration_t& iter
        );

        bool try_start_system(
            std::shared_ptr<base_system_t>& system,
            const std::shared_ptr<misc::worker_t>& worker
        );

        bool try_trigger_system(
            std::shared_ptr<base_system_t>& system,
            const std::shared_ptr<misc::worker_t>& worker,
            const iteration_t& iter,
            const event_t& event
        );

        bool try_update_system(
            std::shared_ptr<base_system_t>& system,
            const system_group_t& group,
            const std::shared_ptr<misc::worker_t>& worker,
            const iteration_t& iter
        );

        void try_stop_system(
            std::shared_ptr<base_system_t>& system,
            const std::shared_ptr<misc::worker_t>& worker,
            const iteration_t& iter
        );

    };

}
