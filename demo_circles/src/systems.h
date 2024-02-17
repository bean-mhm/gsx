#pragma once

#include <vector>

#include "gsx/gsx.h"

#include "components.h"

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

    virtual void on_update(
        ecs::world_t& world,
        const ecs::iteration_t& iter
    ) override;

};

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

    virtual void on_update(
        ecs::world_t& world,
        const ecs::iteration_t& iter
    ) override;

};
