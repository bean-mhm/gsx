#pragma once

#include "tef.h"

#include "components.h"
#include "utils.h"

// Template for an empty system
struct s_empty : tef::base_system_t
{
    s_empty(const std::string& name);
    virtual ~s_empty();

    // Called when the world starts running
    virtual void on_start(tef::world_t& world) override;

    // Called every iteration
    virtual void on_update(tef::world_t& world, const tef::world_iteration_t& iter) override;

    // Called when triggered by an event
    virtual void on_trigger(tef::world_t& world, const tef::world_iteration_t& iter, const tef::event_t& event) override;

    // Called when the world stops running
    virtual void on_stop(tef::world_t& world, const tef::world_iteration_t& iter) override;
};

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
