#include "systems.h"

// STD
#include <string>
#include <algorithm>
#include <execution>
#include <cstdint>

// OpenMP
#include <omp.h>

// Internal
#include "constants.h"
#include "components.h"

math::vec2 get_point_of_attraction(float time)
{
    float a = .8f * time;
    return .7f * math::vec2(math::cos(a), math::sin(a));
}

// Signed distance from the edges of the colliders
// Note: This function must be identical to its GLSL version in plane_src_frag.
float sd_colliders(math::vec2 p, float time)
{
    float d = 1e9;

    // Walls (bounds)
    const math::vec2 min_pos = math::vec2(-.9);
    const math::vec2 max_pos = math::vec2(.9);
    d = math::min(d, p.x - min_pos.x);
    d = math::min(d, p.y - min_pos.y);
    d = math::min(d, max_pos.x - p.x);
    d = math::min(d, max_pos.y - p.y);

    // Circle
    math::vec2 center = math::vec2(math::sin(time) * .4f, 0);
    d = math::min(d, math::distance(p, center) - .15f);

    return d;
}

// s_boids

s_boids::s_boids(const std::string& name, int32_t update_order, bool run_on_caller_thread)
    : tef::base_system_t(name, update_order, run_on_caller_thread)
{}

s_boids::~s_boids()
{}

void s_boids::on_update(tef::world_t& world, const tef::world_iteration_t& iter)
{
    const float dt = math::min(iter.dt, 0.02f);

    c_boid* boids; size_t num_boids;
    world.get_components_of_type<c_boid>(boids, num_boids);

#pragma omp parallel for
    for (int i = 0; i < num_boids; i++)
    {
        c_boid& boid = boids[i];

        // Weighted average of the neighbor velocities
        math::vec2 avg_vel(0);

        // Iterate through the neighbors
        for (int j = 0; j < num_boids; j++)
        {
            if (i == j) continue;

            // Info about the other boid
            const c_boid& other = boids[j];
            math::vec2 this_to_other = other.pos - boid.pos;
            float dist_sqr = math::dot(this_to_other, this_to_other);
            if (dist_sqr > boids_attention_sqr) continue;
            float dist = math::sqrt(dist_sqr);

            // Steer away from nearby boids
            if (
                math::dot(
                    math::normalize(boid.vel),
                    math::normalize(other.vel)
                ) > math::cos(1.1f))
            {
                // How much do I steer away?
                float fac = 1.f - math::clamp01(dist / boids_attention);

                // Steer
                float angle = math::radians(20.f * fac * dt);
                boid.vel = math::transform::apply_vector2D(
                    math::transform::rotate2D_2x2(angle),
                    boid.vel
                );

                // Move away
                boid.vel -= 5.f * fac * dt * this_to_other;
            }

            // Update the weighted average velocity
            float weight = 1.f - math::clamp01(dist / boids_attention);
            avg_vel += weight * other.vel;
        }

        // Try to go in the same direction as the neighbors
        float lensqr_avg_vel = math::dot(avg_vel, avg_vel);
        if (lensqr_avg_vel > 0)
        {
            boid.vel = math::mix(boid.vel, avg_vel, math::min(.3f * dt, 1.f));
        }

        // Try to follow the point of attraction
        math::vec2 poa = get_point_of_attraction(iter.time);
        boid.vel = math::mix(boid.vel, (poa - boid.pos), math::min(dt, 1.f));

        // Constant speed
        boid.vel = boids_speed * math::normalize(boid.vel);

        // Update position
        boid.pos += boid.vel * dt;

        // Get away from the colliders
        {
            // Signed distance
            float sd = sd_colliders(boid.pos, iter.time);

            // Normal
            math::vec2 normal = math::normalize(math::vec2(
                sd_colliders(boid.pos + math::vec2(.001, 0), iter.time) - sd,
                sd_colliders(boid.pos + math::vec2(0, .001), iter.time) - sd
            ));

            // If inside
            if (sd < 0)
            {
                // Snap to outside
                boid.pos += (.001f - sd) * normal;

                // Bounce
                boid.vel = math::reflect(boid.vel, normal);
            }

            // Steer away
            float pd = math::max(0.f, sd);
            float angle = math::radians(-50.f * math::exp(-15.f * pd) * dt);
            boid.vel = math::transform::apply_vector2D(
                math::transform::rotate2D_2x2(angle),
                boid.vel
            );

            // Move away
            float force = 1. / (100. * pd * pd + .1);
            boid.vel += force * dt * normal;
        }
    }
}

// s_rendering

s_rendering::s_rendering(
    const std::string& name,
    int32_t update_order,
    bool run_on_caller_thread,
    GLFWwindow* window
)
    : tef::base_system_t(name, update_order, run_on_caller_thread), window(window)
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
        glUniform2f(
            location,
            (float)width / math::min(width, height),
            (float)height / math::min(width, height)
        );
    }
    {
        GLint location = glGetUniformLocation(plane_shader_program, "px2uv");
        glUniform1f(location, 2.f / math::min(width, height));
    }
    {
        GLint location = glGetUniformLocation(plane_shader_program, "time");
        glUniform1f(location, iter.time);
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
        glUniform2f(
            location,
            (float)width / math::min(width, height),
            (float)height / math::min(width, height)
        );
    }
    {
        GLint location = glGetUniformLocation(boids_shader_program, "boids_size");
        glUniform1f(location, boids_size);
    }
    {
        GLint location = glGetUniformLocation(boids_shader_program, "px2uv");
        glUniform1f(location, (2.f / math::min(width, height)) / boids_size);
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
