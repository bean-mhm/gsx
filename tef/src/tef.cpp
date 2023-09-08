#include "../include/tef.h"

#include <thread>

namespace tef
{

    event_t::event_t(event_type_t type, const std::any& data)
        : type(type), data(data)
    {}

    system_t::system_t(const std::string& name)
        : name(name)
    {}

    system_t::~system_t()
    {}

    void system_t::on_start(world_t& world)
    {}

    void system_t::on_update(world_t& world, const world_iteration_t& iter)
    {}

    void system_t::on_trigger(world_t& world, const world_iteration_t& iter, const event_t& event)
    {}

    void system_t::on_stop(world_t& world, const world_iteration_t& iter)
    {}

    world_t::world_t(const std::string& name)
        : name(name)
    {}

    void world_t::enqueue_event(const event_t& event)
    {
        events.push_back(event);
    }

    void world_t::remove_components_owned_by(entity_t owner)
    {
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
        comp_map.clear();
    }

    std::shared_ptr<system_t> world_t::get_system_named(const std::string& name)
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

    std::vector<std::shared_ptr<system_t>>& world_t::get_systems()
    {
        return systems;
    }

    void world_t::add_system(const std::shared_ptr<system_t>& system)
    {
        systems.push_back(system);
    }

    void world_t::remove_system(const std::string& name)
    {
        for (size_t i = 0; i < systems.size(); i++)
        {
            if (systems[i]->name == name)
            {
                vec_remove(systems, i);
                break;
            }
        }
    }

    void world_t::remove_systems()
    {
        vec_clear(systems);
    }

    void world_t::run(float max_update_rate)
    {
        using namespace std::chrono;

        // Initialization
        stop(true);
        running = true;
        should_stop = false;
        const float min_dt = (max_update_rate == 0) ? 0 : (1.f / max_update_rate);

        // Start
        for (auto& system : systems)
        {
            system->on_start(*this);
        }

        // Iteration info
        world_iteration_t iter;
        auto time_start = high_resolution_clock::now();
        auto time_last_iter = time_start;

        // Loop
        while (!should_stop)
        {
            // Update
            for (auto& system : systems)
            {
                system->on_update(*this, iter);
            }

            // Events
            while (!events.empty())
            {
                const event_t& event = events.front();
                events.pop_front();

                for (auto& system : systems)
                {
                    if (system->triggers.contains(event.type))
                    {
                        system->on_trigger(*this, iter, event);
                    }
                }
            }

            // Sleep if needed
            float time_left = min_dt - elapsed_sec(time_last_iter);
            if (time_left > 0)
            {
                std::this_thread::sleep_for(std::chrono::nanoseconds((uint64_t)(time_left * 1e9f)));
            }

            // Iteration info
            iter.i++;
            iter.elapsed = elapsed_sec(time_start);
            iter.dt = elapsed_sec(time_last_iter);
            time_last_iter = high_resolution_clock::now();
        }

        // Stop in reverse order. The first system added will be started first and it will be
        // stopped at the end.
        for (int64_t i = systems.size() - 1; i >= 0; i--)
        {
            systems[i]->on_stop(*this, iter);
        }

        running = false;
    }

    void world_t::stop(bool wait)
    {
        should_stop = true;

        if (!wait)
            return;

        while (running)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
        }
    }

}
