#pragma once

// STD
#include <vector>

// GSX
#include "gsx/gsx.h"

// Internal
#include "components.h"
#include "gl_utils.h"

// Boid attractor system
class attractor_system_t : public ecs::base_system_t
{
public:
    attractor_system_t(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        std::vector<attractor_t>& attractors
    );
    virtual ~attractor_system_t() = default;

    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;

private:
    std::vector<attractor_t>& attractors;

};

// Boid simulation system
class boid_system_t : public ecs::base_system_t
{
public:
    boid_system_t(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        spatial::base_container_2d_t<boid_t>& boids,
        std::vector<attractor_t>& attractors
    );
    virtual ~boid_system_t() = default;

    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;

private:
    spatial::base_container_2d_t<boid_t>& boids;
    std::vector<attractor_t>& attractors;

};

// OpenGL render system
class render_system_t : public ecs::base_system_t
{
public:
    render_system_t(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        GLFWwindow* window,
        spatial::base_container_2d_t<boid_t>& boids
    );
    virtual ~render_system_t() = default;

    virtual void on_start(ecs::world_t& world) override;
    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;
    virtual void on_stop(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;

private:
    spatial::base_container_2d_t<boid_t>& boids;

    GLFWwindow* window;

    GLuint plane_vao = 0;
    GLuint plane_vbo = 0;
    GLuint plane_ebo = 0;
    GLuint plane_vert_shader = 0;
    GLuint plane_frag_shader = 0;
    GLuint plane_shader_program = 0;

    GLuint boid_vao = 0;
    GLuint boid_vbo = 0;
    GLuint boid_vert_shader = 0;
    GLuint boid_geo_shader = 0;
    GLuint boid_frag_shader = 0;
    GLuint boid_shader_program = 0;

};
