#pragma once

#include "tef.h"

#include "components.h"
#include "utils.h"

// Movement system
struct s_movement : tef::base_system_t
{
    s_movement(const std::string& name);
    virtual ~s_movement();
    virtual void on_update(tef::world_t& world, const tef::world_iteration_t& iter) override;
};

// Circle renderer system (ASCII output to the console)
struct s_circle_renderer : tef::base_system_t
{
    s_circle_renderer(const std::string& name);
    virtual ~s_circle_renderer();
    virtual void on_update(tef::world_t& world, const tef::world_iteration_t& iter) override;
};
