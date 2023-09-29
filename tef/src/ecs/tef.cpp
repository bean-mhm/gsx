#include "tef.h"

// STD
#include <stdexcept>

namespace tef::ecs
{

    base_system_t::base_system_t(
        const std::string& name,
        int32_t update_order,
        bool run_on_caller_thread
    )
        : name(name), update_order(update_order), run_on_caller_thread(run_on_caller_thread)
    {}

    base_system_t::~base_system_t()
    {}

    void base_system_t::on_start(world_t& world)
    {}

    void base_system_t::on_update(world_t& world, const world_iteration_t& iter)
    {}

    void base_system_t::on_trigger(world_t& world, const world_iteration_t& iter, const event_t& event)
    {}

    void base_system_t::on_stop(world_t& world, const world_iteration_t& iter)
    {}

    world_t::world_t(
        const std::string& name,
        log_level_t max_log_level,
        std::shared_ptr<base_logger_t> logger
    )
        : name(name),
        max_log_level(max_log_level),
        logger(logger)
    {
        if (logger == nullptr)
            throw std::runtime_error("The world logger must not be null.");

        tef_log(this, log_level_t::info, "World created");
    }

    world_t::~world_t()
    {
        tef_log(this, log_level_t::info, "World destroyed");
    }

    void world_t::log(log_level_t log_level, const std::string& message)
    {
        tef_log(this, log_level, message);
    }

    void world_t::enqueue_event(const event_t& event)
    {
        tef_log(this, log_level_t::verbose,
            "Enqueueing an event of type " + std::to_string(event.type));

        std::scoped_lock lock(mutex_events);
        events.push_back(event);
    }

    void world_t::remove_components_owned_by(entity_t owner)
    {
        tef_log(this, log_level_t::verbose,
            "Removing all components owned by " + std::to_string(owner));

        // Iterate over all component types
        for (auto& v : comp_map)
        {
            // Iterate over components with the same type
            component_list_t& cl = v.second;
            for (size_t i = 0; i < cl.data.size();)
            {
                // Check the owner
                if (((base_component_t*)(&cl.data[i]))->owner == owner)
                {
                    // Erase
                    cl.data.erase(
                        cl.data.begin() + i,
                        cl.data.begin() + i + cl.stride
                    );
                }
                else
                {
                    // Continue iterating
                    i += cl.stride;
                }
            }
        }
    }

    void world_t::remove_components()
    {
        tef_log(this, log_level_t::verbose, "Removing all components");
        comp_map.clear();
    }

    std::shared_ptr<base_system_t> world_t::get_system_named(const std::string& name)
    {
        for (auto& system : systems)
        {
            if (system->name == name)
            {
                return system;
            }
        }
        return nullptr;
    }

    void world_t::add_system(const std::shared_ptr<base_system_t>& system)
    {
        tef_log(this, log_level_t::verbose, std::format(
            "Adding a new system named \"{}\"",
            system->name
        ));

        systems.push_back(system);
    }

    void world_t::remove_system_named(const std::string& sname)
    {
        tef_log(this, log_level_t::verbose, std::format(
            "Removing the first system named \"{}\"",
            sname
        ));

        for (size_t i = 0; i < systems.size(); i++)
        {
            if (systems[i]->name == sname)
            {
                misc::vec_remove(systems, i);
                break;
            }
        }
    }

    void world_t::remove_systems_named(const std::string& sname)
    {
        tef_log(this, log_level_t::verbose, std::format(
            "Removing all systems named \"{}\"",
            sname
        ));

        for (size_t i = 0; i < systems.size();)
        {
            if (systems[i]->name == sname)
            {
                misc::vec_remove(systems, i);
            }
            else
            {
                i++;
            }
        }
    }

    void world_t::remove_systems()
    {
        tef_log(this, log_level_t::verbose, "Removing all systems");
        misc::vec_clear(systems);
    }

    void world_t::run(const float max_update_rate, const float max_run_time)
    {
        tef_log(this, log_level_t::info, std::format(
            "Preparing to run (max_update_rate = {:.3f} iterations/s, max_run_time = {:.3f} s)",
            max_update_rate,
            max_run_time
        ));

        // If the world is already running, wait for it to stop
        stop(false);
        std::scoped_lock lock(mutex_run);
        should_stop = false;

        // Make a copy of the system list and only ever work with the copied list.
        auto systems_copy = systems;

        // Prepare system groups and workers
        worker_map_t worker_map;
        std::vector<system_group_t> system_groups;
        prepare_system_groups_and_workers(systems_copy, system_groups, worker_map);

        start_systems(systems_copy, worker_map);

        tef_log(this, log_level_t::info, "Starting the loop");

        // Iteration info
        world_iteration_t iter;
        auto time_start = std::chrono::high_resolution_clock::now();
        auto time_last_iter = time_start;
        const float min_dt = (max_update_rate == 0) ? 0 : (1.f / max_update_rate);

        while (!should_stop)
        {
            tef_log(this, log_level_t::verbose, std::format(
                "Loop iteration {} (elapsed = {:.3f} s, dt = {:.3f} s)",
                iter.i, iter.time, iter.dt
            ));

            process_events(systems_copy, worker_map, iter);

            update_systems(system_groups, worker_map, iter);

            // Don't go faster than the maximum update rate
            float time_left = min_dt - misc::elapsed_sec(time_last_iter);
            if (time_left > 0)
            {
                std::this_thread::sleep_for(
                    std::chrono::nanoseconds((uint64_t)(time_left * 1e9f))
                );
            }

            // Iteration info
            iter.i++;
            iter.time = misc::elapsed_sec(time_start);
            iter.dt = misc::elapsed_sec(time_last_iter);
            time_last_iter = std::chrono::high_resolution_clock::now();

            // Stop running if the maximum run time is exceeded
            if (max_run_time != 0 && iter.time > max_run_time)
            {
                tef_log(this, log_level_t::info,
                    "Breaking the loop because the maximum run time was exceeded");

                break;
            }
        }

        stop_systems(systems_copy, worker_map, iter);

        tef_log(this, log_level_t::info, "Stopped running");
    }

    void world_t::stop(bool wait)
    {
        tef_log(this, log_level_t::info, std::format(
            "Signaling the world to stop running (wait = {})",
            wait
        ));

        should_stop = true;
        if (wait)
        {
            std::scoped_lock lock(mutex_run);
        }
    }

    void world_t::prepare_system_groups_and_workers(
        const std::vector<std::shared_ptr<base_system_t>>& systems_copy,
        std::vector<system_group_t>& out_system_groups,
        worker_map_t& out_worker_map
    )
    {
        tef_log(this, log_level_t::info, "Preparing system groups and workers");

        // Make a sorted and unique set of the update order values of all systems
        std::set<int32_t, std::less<int32_t>> update_orders;
        for (const auto& system : systems_copy)
        {
            update_orders.insert(system->update_order);
        }

        // Iterate over the sorted and unique update order values
        for (auto curr_order : update_orders)
        {
            // Make a system group
            system_group_t group;
            group.update_order = curr_order;

            // Gather every system in the world with the current update order
            for (const auto& system : systems_copy)
            {
                if (system->update_order == curr_order)
                {
                    group.systems.push_back(system);
                }
            }

            // Prepare a worker for every system in the group, unless it wants to be updated
            // on the same thread that's running the world. If the group contains only 1
            // system, then there will also be no paralellization and no need for a worker.
            if (group.systems.size() > 1)
            {
                for (const auto& system : group.systems)
                {
                    if (system->run_on_caller_thread)
                    {
                        out_worker_map[system.get()] = nullptr;
                    }
                    else
                    {
                        out_worker_map[system.get()] = std::make_shared<misc::worker_t>(
                            out_worker_map.size()
                        );
                    }
                }
            }
            else if (group.systems.size() == 1)
            {
                out_worker_map[group.systems[0].get()] = nullptr;
            }

            // Add the group to the list
            out_system_groups.push_back(group);
        }
    }

    void world_t::start_systems(
        const std::vector<std::shared_ptr<base_system_t>>& systems_copy,
        worker_map_t& worker_map
    )
    {
        // Start the systems in serial in the order in which they were added
        for (const auto& system : systems_copy)
        {
            const auto& worker = worker_map[system.get()];
            if (worker)
            {
                worker->enqueue(
                    [this, &system, &worker]()
                    {
                        tef_log(this, log_level_t::info, std::format(
                            "Starting system named \"{}\" on worker thread #{}",
                            system->name,
                            worker->id
                        ));

                        system->on_start(*this);
                    }
                );
                worker->wait();
            }
            else
            {
                tef_log(this, log_level_t::info, std::format(
                    "Starting system named \"{}\" on the world runner thread",
                    system->name
                ));

                system->on_start(*this);
            }
        }
    }

    void world_t::process_events(
        const std::vector<std::shared_ptr<base_system_t>>& systems_copy,
        worker_map_t& worker_map,
        const world_iteration_t& iter
    )
    {
        std::unique_lock lock(mutex_events);
        while (!events.empty())
        {
            auto event = events.front();
            events.pop_front();
            lock.unlock();
            for (const auto& system : systems_copy)
            {
                if (system->triggers.contains(event.type))
                {
                    const auto& worker = worker_map[system.get()];
                    if (worker)
                    {
                        worker->enqueue(
                            [this, &iter, &system, &event, &worker]()
                            {
                                tef_log(this, log_level_t::verbose, std::format(
                                    "Using event of type {} to trigger system named \"{}\" "
                                    "on worker thread #{}",
                                    event.type,
                                    system->name,
                                    worker->id
                                ));

                                system->on_trigger(*this, iter, event);
                            }
                        );
                        worker->wait();
                    }
                    else
                    {
                        tef_log(this, log_level_t::verbose, std::format(
                            "Using event of type {} to trigger system named \"{}\" on the "
                            "world runner thread",
                            event.type,
                            system->name
                        ));

                        system->on_trigger(*this, iter, event);
                    }
                }
            }
            lock.lock();
        }
    }

    void world_t::update_systems(
        std::vector<system_group_t>& system_groups,
        worker_map_t& worker_map,
        const world_iteration_t& iter
    )
    {
        for (const auto& group : system_groups)
        {
            tef_log(this, log_level_t::verbose, std::format(
                "Updating {} system(s) at order {}",
                group.systems.size(),
                group.update_order
            ));

            // First, update every system in the group that needs to run on a worker thread.
            for (const auto& system : group.systems)
            {
                const auto& worker = worker_map[system.get()];
                if (worker)
                {
                    worker->enqueue(
                        [this, &iter, &group, &system, &worker]()
                        {
                            tef_log(this, log_level_t::verbose, std::format(
                                "Updating system named \"{}\" at order {} on worker thread #{}",
                                system->name,
                                group.update_order,
                                worker->id
                            ));

                            system->on_update(*this, iter);
                        }
                    );
                }
            }

            // Then, update every system in the group that needs to run on this thread.
            for (const auto& system : group.systems)
            {
                const auto& worker = worker_map[system.get()];
                if (!worker)
                {
                    tef_log(this, log_level_t::verbose, std::format(
                        "Updating system named \"{}\" at order {} on the world runner thread",
                        system->name,
                        group.update_order
                    ));

                    system->on_update(*this, iter);
                }
            }

            // Wait for all the workers to finish their job
            for (const auto& v : worker_map)
            {
                const auto& worker = v.second;
                if (worker)
                {
                    worker->wait();
                }
            }
        }
    }

    void world_t::stop_systems(
        const std::vector<std::shared_ptr<base_system_t>>& systems_copy,
        worker_map_t& worker_map,
        const world_iteration_t& iter
    )
    {
        // Stop the systems in serial in the order opposite to that in which they were added. The
        // first system added will be started first and it will be stopped at the end.
        for (int64_t i = systems_copy.size() - 1; i >= 0; i--)
        {
            const auto& system = systems_copy[i];
            const auto& worker = worker_map[system.get()];
            if (worker)
            {
                worker->enqueue(
                    [this, &iter, &system, &worker]()
                    {
                        tef_log(this, log_level_t::info, std::format(
                            "Stopping system named \"{}\" on worker thread #{}",
                            system->name,
                            worker->id
                        ));

                        system->on_stop(*this, iter);
                    }
                );
                worker->wait();
            }
            else
            {
                tef_log(this, log_level_t::info, std::format(
                    "Stopping system named \"{}\" on the world runner thread",
                    system->name
                ));

                system->on_stop(*this, iter);
            }
        }
    }

}
