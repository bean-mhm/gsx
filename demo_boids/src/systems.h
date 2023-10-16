#pragma once

// ECX
#include "ecx/ecx.h"

// Internal
#include "components.h"
#include "gl_utils.h"

// Boid simulation system
struct s_boids : ecs::base_system_t
{
    spatial::base_container_2d_t<c_boid>& boids;

    s_boids(
        const std::string& name,
        i32 update_order,
        bool run_on_caller_thread,
        spatial::base_container_2d_t<c_boid>& boids
    );
    virtual ~s_boids() = default;

    virtual void on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter) override;
};

// OpenGL rendering system
struct s_rendering : ecs::base_system_t
{
    spatial::base_container_2d_t<c_boid>& boids;

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
    GLFWwindow* window;

    GLuint plane_vao = 0;
    GLuint plane_vbo = 0;
    GLuint plane_ebo = 0;
    GLuint plane_vert_shader = 0;
    GLuint plane_frag_shader = 0;
    GLuint plane_shader_program = 0;

    GLuint boids_vao = 0;
    GLuint boids_vbo = 0;
    GLuint boids_vert_shader = 0;
    GLuint boids_geo_shader = 0;
    GLuint boids_frag_shader = 0;
    GLuint boids_shader_program = 0;

};
