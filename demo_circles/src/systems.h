#pragma once

// STD
#include <vector>

// GSX
#include "gsx/gsx.h"

// Internal
#include "components.h"

// Movement system
class s_movement : public ecs::base_system_t
{
public:
    std::vector<c_transform>& transforms;

    s_movement(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        std::vector<c_transform>& transforms
    );
    virtual ~s_movement() = default;

    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;

};

// Circle renderer system (ASCII output to the console)
class s_circle_renderer : public ecs::base_system_t
{
public:
    std::vector<c_transform>& transforms;
    std::vector<c_circle>& circles;

    s_circle_renderer(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        std::vector<c_transform>& transforms,
        std::vector<c_circle>& circles
    );
    virtual ~s_circle_renderer() = default;

    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;

};
