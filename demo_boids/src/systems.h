#pragma once

#include <vector>

#include "gsx/gsx.h"

#include "components.h"
#include "gl_utils.h"

class attractor_system_t : public ecs::base_system_t
{
public:
    attractor_system_t(
        const std::string& name,
        const ecs::execution_scheme_t& exec_scheme,
        std::vector<attractor_t>& attractors
    );
    virtual ~attractor_system_t() = default;

    virtual void on_update(
        ecs::world_t& world,
        const ecs::iteration_t& iter
    ) override;

private:
    std::vector<attractor_t>& attractors;

};

class boid_system_t : public ecs::base_system_t
{
public:
    boid_system_t(
        const std::string& name,
        const ecs::execution_scheme_t& exec_scheme,
        spatial::base_structure_2d_t<boid_t>& boids,
        std::vector<attractor_t>& attractors
    );
    virtual ~boid_system_t() = default;

    virtual void on_update(
        ecs::world_t& world,
        const ecs::iteration_t& iter
    ) override;

private:
    spatial::base_structure_2d_t<boid_t>& boids;
    std::vector<attractor_t>& attractors;

};

class render_system_t : public ecs::base_system_t
{
public:
    render_system_t(
        const std::string& name,
        const ecs::execution_scheme_t& exec_scheme,
        GLFWwindow* window,
        spatial::base_structure_2d_t<boid_t>& boids
    );
    virtual ~render_system_t() = default;

    virtual void on_start(ecs::world_t& world) override;
    virtual void on_update(
        ecs::world_t& world,
        const ecs::iteration_t& iter
    ) override;
    virtual void on_stop(
        ecs::world_t& world,
        const ecs::iteration_t& iter
    ) override;

private:
    spatial::base_structure_2d_t<boid_t>& boids;

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
