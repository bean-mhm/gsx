#include "system.h"

namespace gsx::ecs
{

    base_system_t::base_system_t(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread
    )
        : name(name), update_order(update_order), run_on_caller_thread(run_on_caller_thread)
    {}

    base_system_t::~base_system_t()
    {}

    void base_system_t::on_start(world_t& world)
    {}

    void base_system_t::on_update(world_t& world, const world_t::iter_t& iter)
    {}

    void base_system_t::on_trigger(world_t& world, const world_t::iter_t& iter, const event_t& event)
    {}

    void base_system_t::on_stop(world_t& world, const world_t::iter_t& iter)
    {}

}