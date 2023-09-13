#include "tef.h"

#include <stdexcept>

namespace tef
{

    event_t::event_t(event_type_t type, const std::any& data)
        : type(type), data(data)
    {}

    base_system_t::base_system_t(const std::string& name)
        : name(name)
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

    world_t::world_t(const std::string& name, std::shared_ptr<base_logger_t> logger)
        : name(name), logger(logger)
    {
        if (logger == nullptr)
            throw std::runtime_error("The world logger must not be null.");

        tef_log(log_level_t::info, name, "World created");
    }

    world_t::~world_t()
    {
        tef_log(log_level_t::info, name, "World destroyed");
    }

    void world_t::enqueue_event(const event_t& event)
    {
        tef_log(log_level_t::verbose, name,
            "Enqueueing an event of type " + std::to_string(event.type));

        std::scoped_lock lock(mutex_events);
        events.push_back(event);
    }

    void world_t::remove_components_owned_by(entity_t owner)
    {
        tef_log(log_level_t::verbose, name,
            "Removing all components owned by " + std::to_string(owner));

        // Iterate over all component types
        for (auto& v : comp_map)
        {
            // Iterate over components with the same type
            component_list_t& cl = v.second;
            size_t nbytes_already_erased = 0;
            for (size_t i = 0; i < cl.data.size(); i += cl.stride)
            {
                // ID of the owner entity
                entity_t owner = *(entity_t*)(&cl.data[i]);
                if (owner == owner)
                {
                    // Erase
                    cl.data.erase(
                        cl.data.begin() + i - nbytes_already_erased,
                        cl.data.begin() + i + cl.stride - nbytes_already_erased
                    );
                    nbytes_already_erased += cl.stride;
                }
            }
        }
    }

    void world_t::remove_components()
    {
        tef_log(log_level_t::verbose, name, "Removing all components");
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
        tef_log(log_level_t::verbose, name, utils::str_format(
            "Adding a new system named \"%s\"",
            system->name.c_str()
        ));

        systems.push_back(system);
    }

    void world_t::remove_system(const std::string& sname)
    {
        tef_log(log_level_t::verbose, name, utils::str_format(
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

    void world_t::remove_systems()
    {
        tef_log(log_level_t::verbose, name, "Removing all systems");
        utils::vec_clear(systems);
    }

    void world_t::run(const float max_update_rate, const float max_run_time)
    {
        using namespace std::chrono;

        // Initialization
        tef_log(log_level_t::info, name, utils::str_format(
            "Preparing to run (max_update_rate = %.3f iterations/s, max_run_time = %.3f s)",
            max_update_rate,
            max_run_time
        ));
        const float min_dt = (max_update_rate == 0) ? 0 : (1.f / max_update_rate);

        // Stop running if the world is already running on another thread
        stop(false);
        std::scoped_lock lock(mutex_run);
        should_stop = false;

        // Start the systems
        for (auto& system : systems)
        {
            tef_log(log_level_t::info, name, utils::str_format(
                "Starting system named \"%s\"",
                system->name.c_str()
            ));

            system->on_start(*this);
        }

        // Iteration info
        world_iteration_t iter;
        auto time_start = high_resolution_clock::now();
        auto time_last_iter = time_start;

        tef_log(log_level_t::info, name, "Starting the loop");

        // Loop
        while (!should_stop)
        {
            tef_log(log_level_t::verbose, name, utils::str_format(
                "Loop iteration %llu (elapsed = %.3f s, dt = %.3f s)",
                iter.i, iter.elapsed, iter.dt
            ));

            // Update the systems
            for (auto& system : systems)
            {
                tef_log(log_level_t::verbose, name, utils::str_format(
                    "Updating system named \"%s\"",
                    system->name.c_str()
                ));

                system->on_update(*this, iter);
            }

            // Events
            while (!events.empty())
            {
                mutex_events.lock();
                event_t event = events.front();
                events.pop_front();
                mutex_events.unlock();

                for (auto& system : systems)
                {
                    if (system->triggers.contains(event.type))
                    {
                        tef_log(log_level_t::verbose, name, utils::str_format(
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
                std::this_thread::sleep_for(
                    std::chrono::nanoseconds((uint64_t)(time_left * 1e9f))
                );
            }

            // Iteration info
            iter.i++;
            iter.elapsed = utils::elapsed_sec(time_start);
            iter.dt = utils::elapsed_sec(time_last_iter);
            time_last_iter = high_resolution_clock::now();

            // Stop running if the maximum run time is exceeded
            if (max_run_time != 0 && iter.elapsed > max_run_time)
            {
                tef_log(log_level_t::info, name,
                    "Breaking the loop because the maximum run time was exceeded");

                break;
            }
        }

        // Stop the systems in reverse order. The first system added will be started first and it
        // will be stopped at the end.
        for (int64_t i = systems.size() - 1; i >= 0; i--)
        {
            tef_log(log_level_t::info, name, utils::str_format(
                "Stopping system named \"%s\"",
                systems[i]->name.c_str()
            ));

            systems[i]->on_stop(*this, iter);
        }

        tef_log(log_level_t::info, name, "Stopped running");
    }

    void world_t::stop(bool wait)
    {
        tef_log(log_level_t::info, name, utils::str_format(
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
