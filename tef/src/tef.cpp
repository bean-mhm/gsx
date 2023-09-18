#include "tef.h"

// STD
#include <stdexcept>

namespace tef
{

    event_t::event_t(event_type_t type, const std::any& data)
        : type(type), data(data)
    {}

    base_system_t::base_system_t(const std::string& name, int32_t update_order)
        : name(name), update_order(update_order)
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
        : name(name), max_log_level(max_log_level), logger(logger)
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
        tef_log(this, log_level_t::verbose, utils::str_format(
            "Adding a new system named \"%s\"",
            system->name.c_str()
        ));

        systems.push_back(system);
    }

    void world_t::remove_system_named(const std::string& sname)
    {
        tef_log(this, log_level_t::verbose, utils::str_format(
            "Removing the first system named \"%s\"",
            sname.c_str()
        ));

        for (size_t i = 0; i < systems.size(); i++)
        {
            if (systems[i]->name == sname)
            {
                utils::vec_remove(systems, i);
                break;
            }
        }
    }

    void world_t::remove_systems_named(const std::string& sname)
    {
        tef_log(this, log_level_t::verbose, utils::str_format(
            "Removing all systems named \"%s\"",
            sname.c_str()
        ));

        for (size_t i = 0; i < systems.size();)
        {
            if (systems[i]->name == sname)
            {
                utils::vec_remove(systems, i);
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
        utils::vec_clear(systems);
    }

    void world_t::run(const float max_update_rate, const float max_run_time)
    {
        using namespace std::chrono;

        // Initialization
        tef_log(this, log_level_t::info, utils::str_format(
            "Preparing to run (max_update_rate = %.3f iterations/s, max_run_time = %.3f s)",
            max_update_rate,
            max_run_time
        ));
        const float min_dt = (max_update_rate == 0) ? 0 : (1.f / max_update_rate);

        // Stop running if the world is already running on another thread
        stop(false);
        std::scoped_lock lock(mutex_run);
        should_stop = false;

        // Make a copy of the system list and only ever work with the copied list. Changes to the
        // original list will not affect this run.
        auto systems_copy = systems;

        // Start the systems
        for (auto& system : systems_copy)
        {
            tef_log(this, log_level_t::info, utils::str_format(
                "Starting system named \"%s\"",
                system->name.c_str()
            ));

            system->on_start(*this);
        }

        // Iteration info
        world_iteration_t iter;
        auto time_start = high_resolution_clock::now();
        auto time_last_iter = time_start;

        tef_log(this, log_level_t::info, "Preparing system groups");

        // A group of systems with identical update order values, all to be updated in parallel
        struct system_group_t
        {
            int32_t update_order;
            std::vector<std::shared_ptr<base_system_t>> systems;
        };

        // A list of system groups to run in serial
        std::vector<system_group_t> system_groups;

        // A list of workers for parallelization
        std::vector<std::shared_ptr<utils::worker_t>> workers;

        // Prepare the system groups
        {
            // Make a sorted and unique set of the update order values of all systems
            std::set<int32_t, std::less<int32_t>> update_orders;
            for (auto& system : systems_copy)
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
                // on the same thread that's running the world. If the group contains less than 2
                // systems, then there will be no paralellization and no need for a worker.
                if (group.systems.size() > 1)
                {
                    for (const auto& system : group.systems)
                    {
                        if (!system->run_on_caller_thread)
                        {
                            workers.push_back(std::make_shared<utils::worker_t>());
                        }
                    }
                }

                // Add the group to the list
                system_groups.push_back(group);
            }
        }

        // If you're confused, here's an example of how the systems could be updated:
        // 1. Update the movement system, the jump system, and the physics system all in parallel.
        // 2. After step 1 is finished, update the collision system.
        // 3. Update the audio system and the render system in parallel.
        // Think of each step as a system group. Notice how the system groups are run in serial,
        // but the systems inside each group are updated together in parallel.

        tef_log(this, log_level_t::info, "Starting the loop");

        // Loop
        while (!should_stop)
        {
            tef_log(this, log_level_t::verbose, utils::str_format(
                "Loop iteration %llu (elapsed = %.3f s, dt = %.3f s)",
                iter.i, iter.elapsed, iter.dt
            ));

            // Index of the current worker to be used for updating a system. This index is global
            // throughout all system groups. This means that each individual system will always be
            // updated on the same thread.
            size_t worker_index = 0;

            // Update the systems
            for (const auto& group : system_groups)
            {
                tef_log(this, log_level_t::verbose, utils::str_format(
                    "Updating %s system(s) at order %s",
                    std::to_string(group.systems.size()).c_str(),
                    std::to_string(group.update_order).c_str()
                ));

                if (group.systems.size() < 1)
                {
                    continue;
                }
                else if (group.systems.size() == 1)
                {
                    tef_log(this, log_level_t::verbose, utils::str_format(
                        "Updating system named \"%s\" at order %s on the world runner thread",
                        group.systems[0]->name.c_str(),
                        std::to_string(group.update_order).c_str()
                    ));

                    // Just a single system to update (no parallelization)
                    group.systems[0]->on_update(*this, iter);
                }
                else
                {
                    // Use the workers to update the systems in parallel
                    for (const auto& system : group.systems)
                    {
                        // If a system wants to be updated on the same thread that's running the
                        // world, don't use a worker for it.
                        if (!system->run_on_caller_thread)
                        {
                            // Get the next worker
                            auto& worker = workers[worker_index++];

                            // Tell the worker to update the system
                            worker->enqueue(
                                [this, &iter, &group, &system]()
                                {
                                    tef_log(this, log_level_t::verbose, utils::str_format(
                                        "Updating system named \"%s\" at order %s on a worker",
                                        system->name.c_str(),
                                        std::to_string(group.update_order).c_str()
                                    ));

                                    system->on_update(*this, iter);
                                }
                            );
                        }
                    }

                    // Use this thread to update systems which want to be updated on the same
                    // thread that's running the world.
                    for (const auto& system : group.systems)
                    {
                        if (system->run_on_caller_thread)
                        {
                            tef_log(this, log_level_t::verbose, utils::str_format(
                                "Updating system named \"%s\" at order %s on the world runner thread",
                                system->name.c_str(),
                                std::to_string(group.update_order).c_str()
                            ));

                            system->on_update(*this, iter);
                        }
                    }

                    // Wait for all the workers to finish their job
                    for (auto& worker : workers)
                    {
                        worker->wait();
                    }
                }
            }

            // Events
            while (!events.empty())
            {
                mutex_events.lock();
                auto event = events.front();
                events.pop_front();
                mutex_events.unlock();

                for (const auto& system : systems_copy)
                {
                    if (system->triggers.contains(event.type))
                    {
                        tef_log(this, log_level_t::verbose, utils::str_format(
                            "Using event of type %s to trigger system named \"%s\"",
                            std::to_string(event.type).c_str(),
                            system->name.c_str()
                        ));
                        system->on_trigger(*this, iter, event);
                    }
                }
            }

            // Don't go faster than the maximum update rate
            float time_left = min_dt - utils::elapsed_sec(time_last_iter);
            if (time_left > 0)
            {
                std::this_thread::sleep_for(nanoseconds((uint64_t)(time_left * 1e9f)));
            }

            // Iteration info
            iter.i++;
            iter.elapsed = utils::elapsed_sec(time_start);
            iter.dt = utils::elapsed_sec(time_last_iter);
            time_last_iter = high_resolution_clock::now();

            // Stop running if the maximum run time is exceeded
            if (max_run_time != 0 && iter.elapsed > max_run_time)
            {
                tef_log(this, log_level_t::info,
                    "Breaking the loop because the maximum run time was exceeded");

                break;
            }
        }

        // Get rid of the workers
        utils::vec_clear(workers);

        // Stop the systems in reverse order. The first system added will be started first and it
        // will be stopped at the end.
        for (int64_t i = systems_copy.size() - 1; i >= 0; i--)
        {
            tef_log(this, log_level_t::info, utils::str_format(
                "Stopping system named \"%s\"",
                systems_copy[i]->name.c_str()
            ));

            systems_copy[i]->on_stop(*this, iter);
        }

        tef_log(this, log_level_t::info, "Stopped running");
    }

    void world_t::stop(bool wait)
    {
        tef_log(this, log_level_t::info, utils::str_format(
            "Signaling the world to stop running (wait = %s)",
            utils::cstr_from_bool(wait)
        ));

        should_stop = true;
        if (wait)
        {
            std::scoped_lock lock(mutex_run);
        }
    }

}
