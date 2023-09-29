#include "system.h"

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

}
