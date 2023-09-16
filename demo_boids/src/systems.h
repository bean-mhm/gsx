#pragma once

// TEF
#include "tef/tef.h"

// Internal
#include "gl_utils.h"

// Boid simulation system
struct s_boids : tef::base_system_t
{
    s_boids(const std::string& name, int32_t update_order);
    virtual ~s_boids();

    virtual void on_update(tef::world_t& world, const tef::world_iteration_t& iter) override;
};

// OpenGL rendering system
struct s_rendering : tef::base_system_t
{
    s_rendering(const std::string& name, int32_t update_order, GLFWwindow* window);
    virtual ~s_rendering();

    virtual void on_start(tef::world_t& world) override;
    virtual void on_update(tef::world_t& world, const tef::world_iteration_t& iter) override;
    virtual void on_stop(tef::world_t& world, const tef::world_iteration_t& iter) override;

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
