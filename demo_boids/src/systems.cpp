#include "systems.h"

#include <omp.h>

#include "constants.h"

using namespace math;

// signed distance from the edges of the colliders
// * this function must be identical to its GLSL version in plane_src_frag.
f32 sd_colliders(vec2 p, f32 time)
{
    f32 d = 1e9;

    // walls (bounds)
    const vec2 min_pos = vec2(-.9);
    const vec2 max_pos = vec2(.9);
    d = min(d, p.x - min_pos.x);
    d = min(d, p.y - min_pos.y);
    d = min(d, max_pos.x - p.x);
    d = min(d, max_pos.y - p.y);

    // circle
    vec2 center = vec2(math::sin(time) * .4f, 0);
    d = min(d, distance(p, center) - .15f);

    return d;
}

attractor_system_t::attractor_system_t(
    const std::string& name,
    const ecs::execution_scheme_t& exec_scheme,
    std::vector<attractor_t>& attractors
)
    : ecs::base_system_t(name, exec_scheme),
    attractors(attractors)
{}

void attractor_system_t::on_update(
    ecs::world_t& world,
    const ecs::iteration_t& iter
)
{
    if (attractors.size() < 1)
        return;

    // rotate the first attractor
    f32 angle = .8f * iter.time;
    attractors[0].pos = .7f * vec2(math::cos(angle), math::sin(angle));
}

boid_system_t::boid_system_t(
    const std::string& name,
    const ecs::execution_scheme_t& exec_scheme,
    spatial::base_structure_2d_t<boid_t>& boids,
    std::vector<attractor_t>& attractors
)
    : ecs::base_system_t(name, exec_scheme),
    boids(boids), attractors(attractors)
{}

void boid_system_t::on_update(
    ecs::world_t& world,
    const ecs::iteration_t& iter
)
{
    const f32 dt = min(iter.dt, 0.02f);

    std::vector<boid_t*> boids_vec;
    boids.query_all(boids_vec);

#pragma omp parallel for
    for (i32 i = 0; i < boids_vec.size(); i++)
    {
        boid_t* boid = boids_vec[i];

        // weighted average of the neighbor velocities
        vec2 avg_vel(0);

        // query the neighbors
        std::vector<boid_t*> neighbors;
        boids.query(
            circle_t(boid->pos, boid_attention_radius),
            neighbors
        );

        // iterate through the neighbors
        for (auto neighbor : neighbors)
        {
            if (neighbor == boid) continue;

            // info about the neighbor
            vec2 this_to_neighbor = neighbor->pos - boid->pos;
            f32 dist_sqr = dot(this_to_neighbor, this_to_neighbor);

            // discard if outside of the attention radius
            if (dist_sqr > boid_attention_radius_sqr)
                continue;

            // distance from the neighbor
            f32 dist = math::sqrt(dist_sqr);

            // steer away from nearby boids
            if (
                dot(
                    normalize(boid->vel),
                    normalize(neighbor->vel)
                ) > math::cos(1.1f))
            {
                // how much do I steer away?
                f32 fac = 1.f - clamp01(dist / boid_attention_radius);

                // steer
                f32 angle = radians(20.f * fac * dt);
                boid->vel = transform::apply_vector_2d(
                    transform::rotate_2d(angle),
                    boid->vel
                );

                // move away
                boid->vel -= 5.f * fac * dt * this_to_neighbor;
            }

            // update the weighted average velocity
            f32 weight = 1.f - clamp01(dist / boid_attention_radius);
            avg_vel += weight * neighbor->vel;
        }

        // try to go in the same direction as the neighbors
        f32 lensqr_avg_vel = dot(avg_vel, avg_vel);
        if (lensqr_avg_vel > 0)
        {
            boid->vel = mix(boid->vel, avg_vel, min(.3f * dt, 1.f));
        }

        // attractors
        for (const auto& attractor : attractors)
        {
            vec2 target_vel = boid_speed * normalize(attractor.pos - boid->pos);
            boid->vel = mix(
                boid->vel,
                target_vel,
                clamp(attractor.strength * dt, -1.f, 1.f)
            );
        }

        // constant speed
        boid->vel = boid_speed * normalize(boid->vel);

        // update position
        boid->pos += boid->vel * dt;

        // get away from the colliders
        {
            // signed distance
            f32 sd = sd_colliders(boid->pos, iter.time);

            // normal
            vec2 normal = normalize(vec2(
                sd_colliders(boid->pos + vec2(.001, 0), iter.time) - sd,
                sd_colliders(boid->pos + vec2(0, .001), iter.time) - sd
            ));

            // if inside
            if (sd < 0)
            {
                // snap to outside
                boid->pos += (.001f - sd) * normal;

                // bounce
                boid->vel = reflect(boid->vel, normal);
            }

            // steer away
            f32 pd = max(0.f, sd);
            f32 angle = radians(-50.f * math::exp(-15.f * pd) * dt);
            boid->vel = transform::apply_vector_2d(
                transform::rotate_2d(angle),
                boid->vel
            );

            // move away
            f32 force = 1. / (100. * pd * pd + .1);
            boid->vel += force * dt * normal;
        }
    }

    boids.rebuild();
}

render_system_t::render_system_t(
    const std::string& name,
    const ecs::execution_scheme_t& exec_scheme,
    GLFWwindow* window,
    spatial::base_structure_2d_t<boid_t>& boids
)
    : ecs::base_system_t(name, exec_scheme),
    window(window), boids(boids)
{}

void render_system_t::on_start(ecs::world_t& world)
{
    // enable alpha blending
    // * blending will happen in sRGB and that's not good at all. however, in
    //   this case I only use it for anti-aliasing at the edges of the boid
    //   shape.
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // plane VAO
    glGenVertexArrays(1, &plane_vao);

    // plane VBO
    glGenBuffers(1, &plane_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
    glBufferData(
        GL_ARRAY_BUFFER, sizeof(plane_vertices), plane_vertices, GL_STATIC_DRAW
    );

    // plane EBO
    glGenBuffers(1, &plane_ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_ebo);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        sizeof(plane_elements),
        plane_elements,
        GL_STATIC_DRAW
    );

    // plane shaders
    make_shader(
        plane_vert_shader,
        "plane vertex shader",
        GL_VERTEX_SHADER,
        plane_src_vert
    );
    make_shader(
        plane_frag_shader,
        "plane fragment shader",
        GL_FRAGMENT_SHADER,
        plane_src_frag
    );

    // plane shader program
    plane_shader_program = glCreateProgram();
    glAttachShader(plane_shader_program, plane_vert_shader);
    glAttachShader(plane_shader_program, plane_frag_shader);
    glBindFragDataLocation(plane_shader_program, 0, "out_col");
    glLinkProgram(plane_shader_program);

    // plane vertex attributes
    glBindVertexArray(plane_vao);
    glBindBuffer(GL_ARRAY_BUFFER, plane_vbo);
    {
        GLint location = glGetAttribLocation(plane_shader_program, "pos");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE,
            2 * sizeof(f32), 0);
    }

    // boid VAO
    glGenVertexArrays(1, &boid_vao);

    // boid VBO
    glGenBuffers(1, &boid_vbo);

    // boid shaders
    make_shader(
        boid_vert_shader,
        "boid vertex shader",
        GL_VERTEX_SHADER,
        boid_src_vert
    );
    make_shader(
        boid_geo_shader,
        "boid geometry shader",
        GL_GEOMETRY_SHADER,
        boid_src_geo
    );
    make_shader(
        boid_frag_shader,
        "boid fragment shader",
        GL_FRAGMENT_SHADER,
        boid_src_frag
    );

    // boid shader program
    boid_shader_program = glCreateProgram();
    glAttachShader(boid_shader_program, boid_vert_shader);
    glAttachShader(boid_shader_program, boid_geo_shader);
    glAttachShader(boid_shader_program, boid_frag_shader);
    glBindFragDataLocation(boid_shader_program, 0, "out_col");
    glLinkProgram(boid_shader_program);

    // boid vertex attributes
    glBindVertexArray(boid_vao);
    glBindBuffer(GL_ARRAY_BUFFER, boid_vbo);
    {
        GLint location = glGetAttribLocation(boid_shader_program, "pos");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE,
            sizeof(boid_t), (void*)offsetof(boid_t, pos));
    }
    {
        GLint location = glGetAttribLocation(boid_shader_program, "vel");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE,
            sizeof(boid_t), (void*)offsetof(boid_t, vel));
    }
}

void render_system_t::on_update(
    ecs::world_t& world,
    const ecs::iteration_t& iter
)
{
    // render dimensions
    i32 width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // clear the screen
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // bind the plane shader program
    glUseProgram(plane_shader_program);

    // plane uniforms
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

    // bind the plane VAO
    glBindVertexArray(plane_vao);

    // bind the plane EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_ebo);

    // draw the plane
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // bind the boid shader program
    glUseProgram(boid_shader_program);

    // boid uniforms
    {
        GLint location = glGetUniformLocation(boid_shader_program, "aspect");
        glUniform2f(
            location,
            (f32)width / min(width, height),
            (f32)height / min(width, height)
        );
    }
    {
        GLint location = glGetUniformLocation(boid_shader_program, "boid_size");
        glUniform1f(location, boid_size);
    }
    {
        GLint location = glGetUniformLocation(boid_shader_program, "px2uv");
        glUniform1f(location, (2.f / min(width, height)) / boid_size);
    }

    // bind the boid VAO
    glBindVertexArray(boid_vao);

    // get a list of all the boids
    std::vector<boid_t> boids_vec;
    boids.query_all(boids_vec);

    // update the boid VBO
    glBindBuffer(GL_ARRAY_BUFFER, boid_vbo);
    glBufferData(
        GL_ARRAY_BUFFER,
        boids_vec.size() * sizeof(boid_t),
        boids_vec.data(),
        GL_DYNAMIC_DRAW
    );

    // draw the boids
    glDrawArrays(GL_POINTS, 0, boids_vec.size());

    // swap front and back buffers
    glfwSwapBuffers(window);

    // poll for and process events
    glfwPollEvents();

    // stop the world if needed
    if (glfwWindowShouldClose(window))
    {
        world.stop(false);
    }
}

void render_system_t::on_stop(ecs::world_t& world, const ecs::iteration_t& iter)
{
    // plane

    glDeleteProgram(plane_shader_program);
    glDeleteShader(plane_frag_shader);
    glDeleteShader(plane_vert_shader);

    glDeleteBuffers(1, &plane_ebo);
    glDeleteBuffers(1, &plane_vbo);
    glDeleteVertexArrays(1, &plane_vao);

    // boids

    glDeleteProgram(boid_shader_program);
    glDeleteShader(boid_frag_shader);
    glDeleteShader(boid_geo_shader);
    glDeleteShader(boid_vert_shader);

    glDeleteBuffers(1, &boid_vbo);
    glDeleteVertexArrays(1, &boid_vao);
}
