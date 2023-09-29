#pragma once

// STD
#include <string>
#include <deque>
#include <vector>
#include <unordered_map>
#include <memory>
#include <mutex>
#include <cstdint>

// Internal
#include "log.h"
#include "event.h"
#include "system.h"
#include "../math/math.h"

namespace tef::ecs
{

    // Information about the current iteration of the world. This will be passed to systems when
    // the world is running.
    struct world_iteration_t
    {
        // Iteration number starting from 0
        uint64_t i = 0;

        // Seconds elapsed since the start
        float time = 0;

        // Seconds elapsed since the last iteration
        float dt = 0;
    };

    // A world for holding and managing a collection of components and systems
    class world_t
    {
    public:
        // A name for the world
        const std::string name;

        // Maximum log level to use
        const log_level_t max_log_level;

        // Mutex for the systems
        // Note: Not used internally, can optionally be used by the developer for synchronization.
        // You might want to use this when adding/removing systems using several threads for
        // example.
        std::mutex mutex_systems;

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
        // Note: Avoid adding or removing systems while the world is running, as it will not
        // affect the current run.
        // Note: Only a single thread can be running the world at a time.
        // Note: Use a max_update_rate of 0 for uncapped update rate.
        // Note: Use a max_run_time of 0 for uncapped run time.
        void run(const float max_update_rate = 0, const float max_run_time = 0);

        // Signal the runner thread to stop, and optionally wait for it if calling from a separate
        // thread. If this is called from the same thread that called run(), wait must be false.
        void stop(bool wait);

    private:
        // A group of systems with identical update order values, all to be updated in parallel.
        // If you're confused, here's an example of how the systems could be updated:
        // 1. Update the movement system, the jump system, and the physics system all in parallel.
        // 2. After step 1 is finished, update the collision system.
        // 3. Update the audio system and the render system in parallel.
        // Think of each step as a system group. Notice how the system groups are run in serial,
        // but the systems inside each group are updated together in parallel.
        struct system_group_t
        {
            int32_t update_order;
            std::vector<std::shared_ptr<base_system_t>> systems;
        };

        // Mapping from a system pointer to a worker thread used to invoke the abtract functions
        // of that system. If the worker is nullptr, then the system must run on the same thread
        // that is running the world.
        using worker_map_t = std::unordered_map<base_system_t*, std::shared_ptr<misc::worker_t>>;

        // Logger
        std::shared_ptr<base_logger_t> logger;

        // Internal mutex for running the world
        std::mutex mutex_run;

        // Internal mutex for the event queue
        std::mutex mutex_events;

        // Event queue
        std::deque<event_t> events;

        // Systems
        std::vector<std::shared_ptr<base_system_t>> systems;

        // Should the world stop running?
        bool should_stop = false;

        void prepare_system_groups_and_workers(
            const std::vector<std::shared_ptr<base_system_t>>& systems_copy,
            std::vector<system_group_t>& out_system_groups,
            worker_map_t& out_worker_map
        );

        void start_systems(
            const std::vector<std::shared_ptr<base_system_t>>& systems_copy,
            worker_map_t& worker_map
        );

        void process_events(
            const std::vector<std::shared_ptr<base_system_t>>& systems_copy,
            worker_map_t& worker_map,
            const world_iteration_t& iter
        );

        void update_systems(
            std::vector<system_group_t>& system_groups,
            worker_map_t& worker_map,
            const world_iteration_t& iter
        );

        void stop_systems(
            const std::vector<std::shared_ptr<base_system_t>>& systems_copy,
            worker_map_t& worker_map,
            const world_iteration_t& iter
        );

    };

}
