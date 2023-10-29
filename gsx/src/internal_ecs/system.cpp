#include "system.h"

namespace gsx::ecs
{

    base_system_t::base_system_t(
        const std::string& name,
        const execution_scheme_t& exec_scheme
    )
        : name(name), exec_scheme(exec_scheme)
    {}

    base_system_t::~base_system_t()
    {}

    void base_system_t::on_start(world_t& world)
    {}

    void base_system_t::on_update(world_t& world, const iteration_t& iter)
    {}

    void base_system_t::on_trigger(world_t& world, const iteration_t& iter, const event_t& event)
    {}

    void base_system_t::on_stop(world_t& world, const iteration_t& iter)
    {}

    execution_scheme_t::execution_scheme_t(i32 update_order, bool run_on_world_thread)
        : update_order(update_order), run_on_world_thread(run_on_world_thread)
    {}

}
