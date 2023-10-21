#pragma once

// STD
#include <vector>

// GSX
#include "gsx/gsx.h"

// Internal
#include "components.h"
#include "gl_utils.h"

// Boid attractors system
class s_attractors : public ecs::base_system_t
{
public:
    s_attractors(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        std::vector<c_attractor>& attractors
    );
    virtual ~s_attractors() = default;

    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;

private:
    std::vector<c_attractor>& attractors;

};

// Boid simulation system
class s_boids : public ecs::base_system_t
{
public:
    s_boids(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        spatial::base_container_2d_t<c_boid>& boids,
        std::vector<c_attractor>& attractors
    );
    virtual ~s_boids() = default;

    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;

private:
    spatial::base_container_2d_t<c_boid>& boids;
    std::vector<c_attractor>& attractors;

};

// OpenGL rendering system
class s_rendering : public ecs::base_system_t
{
public:
    s_rendering(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        GLFWwindow* window,
        spatial::base_container_2d_t<c_boid>& boids
    );
    virtual ~s_rendering() = default;

    virtual void on_start(ecs::world_t& world) override;
    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;
    virtual void on_stop(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;

private:
    spatial::base_container_2d_t<c_boid>& boids;

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
