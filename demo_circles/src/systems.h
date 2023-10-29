#pragma once

// STD
#include <vector>

// GSX
#include "gsx/gsx.h"

// Internal
#include "components.h"

// Movement system
class movement_system_t : public ecs::base_system_t
{
public:
    std::vector<transform_t>& transforms;

    movement_system_t(
        const std::string& name,
        const ecs::execution_scheme_t& exec_scheme,
        std::vector<transform_t>& transforms
    );
    virtual ~movement_system_t() = default;

    virtual void on_update(ecs::world_t& world, const ecs::iteration_t& iter) override;

};

// Circle render system (ASCII output to the console)
class render_system_t : public ecs::base_system_t
{
public:
    std::vector<transform_t>& transforms;
    std::vector<circle_t>& circles;

    render_system_t(
        const std::string& name,
        const ecs::execution_scheme_t& exec_scheme,
        std::vector<transform_t>& transforms,
        std::vector<circle_t>& circles
    );
    virtual ~render_system_t() = default;

    virtual void on_update(ecs::world_t& world, const ecs::iteration_t& iter) override;

};
