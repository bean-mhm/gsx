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

using namespace math;

vec2 get_point_of_attraction(f32 time)
{
    f32 a = .8f * time;
    return .7f * vec2(math::cos(a), math::sin(a));
}

// Signed distance from the edges of the colliders
// Note: This function must be identical to its GLSL version in plane_src_frag.
f32 sd_colliders(vec2 p, f32 time)
{
    f32 d = 1e9;

    // Walls (bounds)
    const vec2 min_pos = vec2(-.9);
    const vec2 max_pos = vec2(.9);
    d = min(d, p.x - min_pos.x);
    d = min(d, p.y - min_pos.y);
    d = min(d, max_pos.x - p.x);
    d = min(d, max_pos.y - p.y);

    // Circle
    vec2 center = vec2(math::sin(time) * .4f, 0);
    d = min(d, distance(p, center) - .15f);

    return d;
}

// s_boids

s_boids::s_boids(
    const std::string& name,
    i32 update_order,
    bool run_on_caller_thread,
    spatial::grid_2d_t<c_boid>& boids
)
    : ecs::base_system_t(name, update_order, run_on_caller_thread),
    boids(boids)
{}

void s_boids::on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter)
{
    const f32 dt = min(iter.dt, 0.02f);

    std::vector<c_boid*> boids_vec;
    boids.query_all(boids_vec);

#pragma omp parallel for
    for (int i = 0; i < boids_vec.size(); i++)
    {
        c_boid* boid = boids_vec[i];

        // Weighted average of the neighbor velocities
        vec2 avg_vel(0);

        // Query the neighbors
        std::vector<c_boid*> neighbors;
        boids.query(
            bounds2(boid->pos - boids_attention, boid->pos + boids_attention),
            neighbors
        );

        // Iterate through the neighbors
        for (auto neighbor : neighbors)
        {
            if (neighbor == boid) continue;

            // Info about the neighbor
            vec2 this_to_neighbor = neighbor->pos - boid->pos;
            f32 dist_sqr = dot(this_to_neighbor, this_to_neighbor);
            if (dist_sqr > boids_attention_sqr) continue;
            f32 dist = math::sqrt(dist_sqr);

            // Steer away from nearby boids
            if (
                dot(
                    normalize(boid->vel),
                    normalize(neighbor->vel)
                ) > math::cos(1.1f))
            {
                // How much do I steer away?
                f32 fac = 1.f - clamp01(dist / boids_attention);

                // Steer
                f32 angle = radians(20.f * fac * dt);
                boid->vel = transform::apply_vector_2d(
                    transform::rotate_2d(angle),
                    boid->vel
                );

                // Move away
                boid->vel -= 5.f * fac * dt * this_to_neighbor;
            }

            // Update the weighted average velocity
            f32 weight = 1.f - clamp01(dist / boids_attention);
            avg_vel += weight * neighbor->vel;
        }

        // Try to go in the same direction as the neighbors
        f32 lensqr_avg_vel = dot(avg_vel, avg_vel);
        if (lensqr_avg_vel > 0)
        {
            boid->vel = mix(boid->vel, avg_vel, min(.3f * dt, 1.f));
        }

        // Try to follow the point of attraction
        vec2 poa = get_point_of_attraction(iter.time);
        boid->vel = mix(boid->vel, (poa - boid->pos), min(dt, 1.f));

        // Constant speed
        boid->vel = boids_speed * normalize(boid->vel);

        // Update position
        boid->pos += boid->vel * dt;

        // Get away from the colliders
        {
            // Signed distance
            f32 sd = sd_colliders(boid->pos, iter.time);

            // Normal
            vec2 normal = normalize(vec2(
                sd_colliders(boid->pos + vec2(.001, 0), iter.time) - sd,
                sd_colliders(boid->pos + vec2(0, .001), iter.time) - sd
            ));

            // If inside
            if (sd < 0)
            {
                // Snap to outside
                boid->pos += (.001f - sd) * normal;

                // Bounce
                boid->vel = reflect(boid->vel, normal);
            }

            // Steer away
            f32 pd = max(0.f, sd);
            f32 angle = radians(-50.f * math::exp(-15.f * pd) * dt);
            boid->vel = transform::apply_vector_2d(
                transform::rotate_2d(angle),
                boid->vel
            );

            // Move away
            f32 force = 1. / (100. * pd * pd + .1);
            boid->vel += force * dt * normal;
        }
    }

    boids.rebuild();
}

// s_rendering

s_rendering::s_rendering(
    const std::string& name,
    i32 update_order,
    bool run_on_caller_thread,
    GLFWwindow* window,
    spatial::grid_2d_t<c_boid>& boids
)
    : ecs::base_system_t(name, update_order, run_on_caller_thread), window(window),
    boids(boids)
{}

void s_rendering::on_start(ecs::world_t& world)
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
            2 * sizeof(f32), 0);
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

void s_rendering::on_update(ecs::world_t& world, const ecs::world_t::iter_t& iter)
{
    // Render dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    f32 aspect = width / (f32)height;

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
            (f32)width / min(width, height),
            (f32)height / min(width, height)
        );
    }
    {
        GLint location = glGetUniformLocation(plane_shader_program, "px2uv");
        glUniform1f(location, 2.f / min(width, height));
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
            (f32)width / min(width, height),
            (f32)height / min(width, height)
        );
    }
    {
        GLint location = glGetUniformLocation(boids_shader_program, "boids_size");
        glUniform1f(location, boids_size);
    }
    {
        GLint location = glGetUniformLocation(boids_shader_program, "px2uv");
        glUniform1f(location, (2.f / min(width, height)) / boids_size);
    }

    // Bind the boids VAO
    glBindVertexArray(boids_vao);

    // Get a list of all the boids
    std::vector<c_boid> boids_vec;
    boids.query_all(boids_vec);

    // Update the boids VBO
    glBindBuffer(GL_ARRAY_BUFFER, boids_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        boids_vec.size() * sizeof(c_boid),
        boids_vec.data(),
        GL_DYNAMIC_DRAW
    );

    // Draw the boids
    glDrawArrays(GL_POINTS, 0, boids_vec.size());

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

void s_rendering::on_stop(ecs::world_t& world, const ecs::world_t::iter_t& iter)
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
