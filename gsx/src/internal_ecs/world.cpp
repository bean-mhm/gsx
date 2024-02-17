#include "world.h"

#include <stdexcept>

#include "system.h"

namespace gsx::ecs
{

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
            throw std::runtime_error("the world logger must not be null");

        gsx_log(this, log_level_t::info, "world created");
    }

    world_t::~world_t()
    {
        gsx_log(this, log_level_t::info, "world destroyed");
    }

    void world_t::log(log_level_t log_level, const std::string& message)
    {
        gsx_log(this, log_level, message);
    }

    void world_t::enqueue_event(const event_t& event)
    {
        gsx_log(this, log_level_t::verbose,
            "enqueueing an event of type " + std::to_string(event.type));

        std::scoped_lock lock(mutex_events);
        events.push_back(event);
    }

    std::shared_ptr<base_system_t> world_t::get_system_named(
        const std::string& name
    )
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
        gsx_log(this, log_level_t::verbose, std::format(
            "adding a new system named \"{}\"",
            system->name
        ));

        systems.push_back(system);
    }

    void world_t::remove_first_system_named(const std::string& sname)
    {
        gsx_log(this, log_level_t::verbose, std::format(
            "removing the first system named \"{}\"",
            sname
        ));

        for (usize i = 0; i < systems.size(); i++)
        {
            if (systems[i]->name == sname)
            {
                misc::vec_remove(systems, i);
                break;
            }
        }
    }

    void world_t::remove_all_systems_named(const std::string& sname)
    {
        gsx_log(this, log_level_t::verbose, std::format(
            "removing all systems named \"{}\"",
            sname
        ));

        for (usize i = 0; i < systems.size();)
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

    void world_t::remove_all_systems()
    {
        gsx_log(this, log_level_t::verbose, "removing all systems");
        misc::vec_clear(systems);
    }

    void world_t::run(const f32 max_update_rate, const f32 max_run_time)
    {
        gsx_log(this, log_level_t::info, "preparing to run");

        if (max_update_rate != 0)
            gsx_log(this, log_level_t::info, std::format(
                "max_update_rate = {:.3f} iterations/s",
                max_update_rate
            ));

        if (max_run_time != 0)
            gsx_log(this, log_level_t::info, std::format(
                "max_run_time = {:.3f} s",
                max_run_time
            ));

        should_stop = true;
        std::scoped_lock lock(mutex_run);
        should_stop = false;

        // make a copy of the system list and only ever work with the copied
        // list.
        auto systems_copy = systems;

        std::vector<system_group_t> system_groups;
        worker_map_t worker_map;
        prepare_system_groups_and_workers(
            systems_copy,
            system_groups,
            worker_map
        );

        bool did_start_all;
        start_systems(systems_copy, worker_map, did_start_all);

        iteration_t iter;
        auto time_start = std::chrono::high_resolution_clock::now();
        auto time_last_iter = time_start;
        const f32 min_dt =
            (max_update_rate == 0)
            ? 0
            : 1.f / max_update_rate;

        if (did_start_all)
        {
            gsx_log(this, log_level_t::info, "starting the loop");

            // start the loop
            while (!should_stop)
            {
                gsx_log(this, log_level_t::verbose, std::format(
                    "loop iteration {} (elapsed = {:.3f} s, dt = {:.3f} s)",
                    iter.i, iter.time, iter.dt
                ));

                bool did_process_all_events = false;
                bool did_update_all = false;

                process_events(
                    systems_copy,
                    worker_map,
                    iter,
                    did_process_all_events
                );

                if (did_process_all_events)
                {
                    update_systems(
                        system_groups,
                        worker_map,
                        iter,
                        did_update_all
                    );
                }

                // don't go faster than the maximum update rate
                f32 time_left = min_dt - misc::elapsed_sec(time_last_iter);
                if (time_left > 0)
                {
                    misc::sleep(time_left);
                }

                // update iter
                iter.i++;
                iter.time = misc::elapsed_sec(time_start);
                iter.dt = misc::elapsed_sec(time_last_iter);
                time_last_iter = std::chrono::high_resolution_clock::now();

                // stop running if one or more system failed to update or get
                // triggered
                if (!did_process_all_events || !did_update_all)
                    break;

                // stop running if the maximum run time is exceeded
                if (max_run_time != 0 && iter.time > max_run_time)
                {
                    gsx_log(
                        this,
                        log_level_t::info,
                        "breaking the loop because the maximum run time was "
                        "exceeded"
                    );
                    break;
                }
            }
        }

        stop_systems(systems_copy, worker_map, iter);

        gsx_log(this, log_level_t::info, "stopped running");
    }

    void world_t::stop(bool wait)
    {
        gsx_log(this, log_level_t::info, std::format(
            "signaling the world to stop running (wait = {})",
            wait
        ));

        should_stop = true;
        if (wait)
        {
            std::scoped_lock lock(mutex_run);
        }
    }

    void world_t::prepare_system_groups_and_workers(
        std::vector<std::shared_ptr<base_system_t>>& systems_copy,
        std::vector<system_group_t>& out_system_groups,
        worker_map_t& out_worker_map
    )
    {
        gsx_log(
            this,
            log_level_t::info,
            "preparing system groups and workers"
        );

        // make a sorted and unique set of the update order values
        std::set<i32, std::less<i32>> update_orders;
        for (auto& system : systems_copy)
        {
            update_orders.insert(system->exec_scheme.update_order);
        }

        // iterate over the sorted and unique update order values
        for (auto curr_order : update_orders)
        {
            // make a system group
            system_group_t group;
            group.update_order = curr_order;

            // gather every system in the world with the current update order
            for (auto& system : systems_copy)
            {
                if (system->exec_scheme.update_order == curr_order)
                {
                    group.systems.push_back(system);
                }
            }

            // prepare a worker for every system in the group, unless it wants
            // to be updated on the same thread that is running the world. if
            // the group contains only 1 system, then there will also be no
            // paralellization and no need for a worker.
            if (group.systems.size() > 1)
            {
                for (auto& system : group.systems)
                {
                    if (system->exec_scheme.run_on_world_thread)
                    {
                        out_worker_map[system.get()] = nullptr;
                    }
                    else
                    {
                        out_worker_map[system.get()] =
                            std::make_shared<misc::worker_t>(
                                out_worker_map.size()
                            );
                    }
                }
            }
            else if (group.systems.size() == 1)
            {
                out_worker_map[group.systems[0].get()] = nullptr;
            }

            // add the group to the list
            out_system_groups.push_back(group);
        }
    }

    void world_t::start_systems(
        std::vector<std::shared_ptr<base_system_t>>& systems_copy,
        worker_map_t& worker_map,
        bool& out_did_start_all
    )
    {
        out_did_start_all = true;

        // start the systems in serial in the order in which they were added
        for (auto& system : systems_copy)
        {
            auto& worker = worker_map[system.get()];
            if (worker)
            {
                worker->enqueue(
                    [this, &system, &worker, &out_did_start_all]()
                    {
                        if (!try_start_system(system, worker))
                        {
                            out_did_start_all = false;
                        }
                    }
                );
                worker->wait();
            }
            else
            {
                if (!try_start_system(system, worker))
                {
                    out_did_start_all = false;
                }
            }
        }
    }

    void world_t::process_events(
        std::vector<std::shared_ptr<base_system_t>>& systems_copy,
        worker_map_t& worker_map,
        const iteration_t& iter,
        bool& out_did_process_all
    )
    {
        out_did_process_all = true;

        std::unique_lock lock(mutex_events);
        while (!events.empty())
        {
            auto event = events.front();
            events.pop_front();
            lock.unlock();
            for (auto& system : systems_copy)
            {
                if (system->triggers.contains(event.type))
                {
                    auto& worker = worker_map[system.get()];
                    if (worker)
                    {
                        worker->enqueue(
                            [
                                this, &system, &worker, &iter, &event,
                                &out_did_process_all
                            ]()
                            {
                                if (!try_trigger_system(
                                    system, worker, iter, event
                                ))
                                {
                                    out_did_process_all = false;
                                }
                            }
                        );
                        worker->wait();
                    }
                    else
                    {
                        if (!try_trigger_system(system, worker, iter, event))
                        {
                            out_did_process_all = false;
                        }
                    }
                }
            }
            lock.lock();
        }
    }

    void world_t::update_systems(
        std::vector<system_group_t>& system_groups,
        worker_map_t& worker_map,
        const iteration_t& iter,
        bool& out_did_update_all
    )
    {
        out_did_update_all = true;

        for (auto& group : system_groups)
        {
            gsx_log(this, log_level_t::verbose, std::format(
                "updating {} system(s) at order {}",
                group.systems.size(),
                group.update_order
            ));

            // first, update every system in the group that needs to run on a
            // worker thread.
            for (auto& system : group.systems)
            {
                auto& worker = worker_map[system.get()];
                if (worker)
                {
                    worker->enqueue(
                        [
                            this, &system, &group, &worker, &iter,
                            &out_did_update_all
                        ]()
                        {
                            if (!try_update_system(system, group, worker, iter))
                            {
                                out_did_update_all = false;
                            }
                        }
                    );
                }
            }

            // then, update every system in the group that needs to run on this
            // thread.
            for (auto& system : group.systems)
            {
                auto& worker = worker_map[system.get()];
                if (!worker)
                {
                    if (!try_update_system(system, group, worker, iter))
                    {
                        out_did_update_all = false;
                    }
                }
            }

            // wait for the workers to finish their job
            for (auto& v : worker_map)
            {
                auto& worker = v.second;
                if (worker)
                {
                    worker->wait();
                }
            }
        }
    }

    void world_t::stop_systems(
        std::vector<std::shared_ptr<base_system_t>>& systems_copy,
        worker_map_t& worker_map,
        const iteration_t& iter
    )
    {
        // stop the systems in serial in the opposite order to that in which
        // they were added. the first system added will be started first and it
        // will be stopped at the end.
        for (isize i = systems_copy.size() - 1; i >= 0; i--)
        {
            auto& system = systems_copy[i];
            auto& worker = worker_map[system.get()];
            if (worker)
            {
                worker->enqueue(
                    [this, &iter, &system, &worker]()
                    {
                        try_stop_system(system, worker, iter);
                    }
                );
                worker->wait();
            }
            else
            {
                try_stop_system(system, worker, iter);
            }
        }
    }

    bool world_t::try_start_system(
        std::shared_ptr<base_system_t>& system,
        const std::shared_ptr<misc::worker_t>& worker
    )
    {
        if (worker)
        {
            gsx_log(this, log_level_t::info, std::format(
                "starting system named \"{}\" on worker thread #{}",
                system->name,
                worker->id
            ));
        }
        else
        {
            gsx_log(this, log_level_t::info, std::format(
                "starting system named \"{}\" on the world runner thread",
                system->name
            ));
        }

        try
        {
            system->on_start(*this);
            return true;
        }
        catch (const std::exception& e)
        {
            gsx_log(this, log_level_t::error, std::format(
                "system named \"{}\" couldn't start: \"{}\"",
                system->name,
                e.what()
            ));
        }

        return false;
    }

    bool world_t::try_trigger_system(
        std::shared_ptr<base_system_t>& system,
        const std::shared_ptr<misc::worker_t>& worker,
        const iteration_t& iter,
        const event_t& event
    )
    {
        if (worker)
        {
            gsx_log(this, log_level_t::verbose, std::format(
                "triggering system named \"{}\" using event of type {} on "
                "worker thread #{}",
                system->name,
                event.type,
                worker->id
            ));
        }
        else
        {
            gsx_log(this, log_level_t::verbose, std::format(
                "triggering system named \"{}\" using event of type {} on the "
                "world runner thread",
                system->name,
                event.type
            ));
        }

        try
        {
            system->on_trigger(*this, iter, event);
            return true;
        }
        catch (const std::exception& e)
        {
            gsx_log(this, log_level_t::error, std::format(
                "system named \"{}\" couldn't be triggered: \"{}\"",
                system->name,
                e.what()
            ));
        }

        return false;
    }

    bool world_t::try_update_system(
        std::shared_ptr<base_system_t>& system,
        const system_group_t& group,
        const std::shared_ptr<misc::worker_t>& worker,
        const iteration_t& iter
    )
    {
        if (worker)
        {
            gsx_log(this, log_level_t::verbose, std::format(
                "updating system named \"{}\" at order {} on worker "
                "thread #{}",
                system->name,
                group.update_order,
                worker->id
            ));
        }
        else
        {
            gsx_log(this, log_level_t::verbose, std::format(
                "updating system named \"{}\" at order {} on the world runner "
                "thread",
                system->name,
                group.update_order
            ));
        }

        try
        {
            system->on_update(*this, iter);
            return true;
        }
        catch (const std::exception& e)
        {
            gsx_log(this, log_level_t::error, std::format(
                "system named \"{}\" couldn't update: \"{}\"",
                system->name,
                e.what()
            ));
        }

        return false;
    }

    void world_t::try_stop_system(
        std::shared_ptr<base_system_t>& system,
        const std::shared_ptr<misc::worker_t>& worker,
        const iteration_t& iter
    )
    {
        if (worker)
        {
            gsx_log(this, log_level_t::info, std::format(
                "stopping system named \"{}\" on worker thread #{}",
                system->name,
                worker->id
            ));
        }
        else
        {
            gsx_log(this, log_level_t::info, std::format(
                "stopping system named \"{}\" on the world runner thread",
                system->name
            ));
        }

        try
        {
            system->on_stop(*this, iter);
        }
        catch (const std::exception& e)
        {
            gsx_log(this, log_level_t::error, std::format(
                "system named \"{}\" couldn't stop: \"{}\"",
                system->name,
                e.what()
            ));
        }
    }

}
