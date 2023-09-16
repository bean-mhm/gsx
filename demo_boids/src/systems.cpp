#include "systems.h"

// STD
#include <string>
#include <algorithm>
#include <cmath>
#include <cstdint>

// GLM
#include "glm/glm.hpp"

// Internal
#include "constants.h"
#include "components.h"

// s_boids

s_boids::s_boids(const std::string& name, int32_t update_order)
    : tef::base_system_t(name, update_order)
{}

s_boids::~s_boids()
{}

void s_boids::on_update(tef::world_t& world, const tef::world_iteration_t& iter)
{
    // Get a list of the boids
    c_boid* boids; size_t num_boids;
    world.get_components_of_type<c_boid>(boids, num_boids);

    // Iterate and update
    for (size_t i = 0; i < num_boids; i++)
    {
        c_boid& boid = boids[i];

        boid.pos += iter.dt * boid.vel;
    }
}

// s_rendering

s_rendering::s_rendering(const std::string& name, int32_t update_order, GLFWwindow* window)
    : tef::base_system_t(name, update_order), window(window)
{}

s_rendering::~s_rendering()
{}

void s_rendering::on_start(tef::world_t& world)
{
    // Enable alpha blending
    // Note: Blending will happen in sRGB and that's not good at all. However, in this case I only
    // use it for anti-aliasing at the edges of the boid shape.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Plane VAO
    glGenVertexArrays(1, &plane_vao);

    // Plane VBO
    glGenBuffers(1, &plane_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW);

    // Plane EBO
    glGenBuffers(1, &plane_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(plane_elements), plane_elements, GL_STATIC_DRAW);

    // Plane shaders
    make_shader(plane_vert_shader, "plane vertex shader", GL_VERTEX_SHADER, plane_src_vert);
    make_shader(plane_frag_shader, "plane fragment shader", GL_FRAGMENT_SHADER, plane_src_frag);

    // Plane shader program
    plane_shader_program = glCreateProgram();
    glAttachShader(plane_shader_program, plane_vert_shader);
    glAttachShader(plane_shader_program, plane_frag_shader);
    glBindFragDataLocation(plane_shader_program, 0, "out_col");
    glLinkProgram(plane_shader_program);

    // Plane vertex attributes
    glBindVertexArray(plane_vao);
    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
    {
        GLint location = glGetAttribLocation(plane_shader_program, "pos");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE,
            2 * sizeof(float), 0);
    }

    // Boids VAO
    glGenVertexArrays(1, &boids_vao);

    // Boids VBO
    glGenBuffers(1, &boids_vbo);

    // Boids shaders
    make_shader(boids_vert_shader, "boids vertex shader", GL_VERTEX_SHADER, boids_src_vert);
    make_shader(boids_geo_shader, "boids geometry shader", GL_GEOMETRY_SHADER, boids_src_geo);
    make_shader(boids_frag_shader, "boids fragment shader", GL_FRAGMENT_SHADER, boids_src_frag);

    // Boids shader program
    boids_shader_program = glCreateProgram();
    glAttachShader(boids_shader_program, boids_vert_shader);
    glAttachShader(boids_shader_program, boids_geo_shader);
    glAttachShader(boids_shader_program, boids_frag_shader);
    glBindFragDataLocation(boids_shader_program, 0, "out_col");
    glLinkProgram(boids_shader_program);

    // Boids vertex attributes
    glBindVertexArray(boids_vao);
    glBindBuffer(GL_ARRAY_BUFFER, boids_vbo);
    {
        GLint location = glGetAttribLocation(boids_shader_program, "pos");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE,
            sizeof(c_boid), (void*)offsetof(c_boid, pos));
    }
    {
        GLint location = glGetAttribLocation(boids_shader_program, "vel");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE,
            sizeof(c_boid), (void*)offsetof(c_boid, vel));
    }
}

void s_rendering::on_update(tef::world_t& world, const tef::world_iteration_t& iter)
{
    // Render dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    float aspect = width / (float)height;

    // Clear the screen
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind the plane shader program
    glUseProgram(plane_shader_program);

    // Plane uniforms
    {
        GLint location = glGetUniformLocation(plane_shader_program, "aspect");
        glUniform2f(location, (float)width / std::min(width, height), (float)height / std::min(width, height));
    }
    {
        GLint location = glGetUniformLocation(plane_shader_program, "px2uv");
        glUniform1f(location, 2.f / std::min(width, height));
    }
    {
        GLint location = glGetUniformLocation(plane_shader_program, "time");
        glUniform1f(location, iter.elapsed);
    }

    // Bind the plane VAO
    glBindVertexArray(plane_vao);

    // Bind the plane EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_ebo);

    // Draw the plane
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Bind the boids shader program
    glUseProgram(boids_shader_program);

    // Boids uniforms
    {
        GLint location = glGetUniformLocation(boids_shader_program, "aspect");
        glUniform2f(location, (float)width / std::min(width, height), (float)height / std::min(width, height));
    }
    {
        GLint location = glGetUniformLocation(boids_shader_program, "boids_size");
        glUniform1f(location, boids_size);
    }
    {
        GLint location = glGetUniformLocation(boids_shader_program, "px2uv");
        glUniform1f(location, (2.f / std::min(width, height)) / boids_size);
    }

    // Bind the boids VAO
    glBindVertexArray(boids_vao);

    // Get a list of the boids
    c_boid* boids; size_t num_boids;
    world.get_components_of_type<c_boid>(boids, num_boids);

    // Update the boids VBO
    glBindBuffer(GL_ARRAY_BUFFER, boids_vbo);
    glBufferData(GL_ARRAY_BUFFER, num_boids * sizeof(c_boid), boids, GL_DYNAMIC_DRAW);

    // Draw the boids
    glDrawArrays(GL_POINTS, 0, num_boids);

    // Swap front and back buffers
    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();

    // Stop the world if needed
    if (glfwWindowShouldClose(window))
    {
        world.stop(false);
    }
}

void s_rendering::on_stop(tef::world_t& world, const tef::world_iteration_t& iter)
{
    // Plane

    glDeleteProgram(plane_shader_program);
    glDeleteShader(plane_frag_shader);
    glDeleteShader(plane_vert_shader);

    glDeleteBuffers(1, &plane_ebo);
    glDeleteBuffers(1, &plane_vbo);
    glDeleteVertexArrays(1, &plane_vao);

    // Boids

    glDeleteProgram(boids_shader_program);
    glDeleteShader(boids_frag_shader);
    glDeleteShader(boids_geo_shader);
    glDeleteShader(boids_vert_shader);

    glDeleteBuffers(1, &boids_vbo);
    glDeleteVertexArrays(1, &boids_vao);
}
