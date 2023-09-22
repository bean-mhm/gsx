#pragma once

// STD
#include <cstdint>

// Internal
#include "gl_utils.h"

// Plane

static const char* plane_src_vert = R"glsl(
    #version 330 core

    uniform vec2 aspect;

    in vec2 pos;

    out vec2 v_uv;

    void main()
    {
        v_uv = pos * aspect;
        gl_Position = vec4(pos, 0, 1);
    }
)glsl";

static const char* plane_src_frag = R"glsl(
    #version 330 core

    uniform float px2uv;
    uniform float time;

    in vec2 v_uv;

    out vec4 out_col;

    float remap01(float inp, float inp_start, float inp_end)
    {
        return clamp((inp - inp_start) / (inp_end - inp_start), 0., 1.);
    }

    // Signed distance from the edges of the colliders
    // Note: This function must be identical to its C++ version in systems.cpp.
    float sd_colliders(vec2 p)
    {
        float d = 1e9;

        // Walls (bounds)
        const vec2 min_pos = vec2(-.9);
        const vec2 max_pos = vec2(.9);
        d = min(d, p.x - min_pos.x);
        d = min(d, p.y - min_pos.y);
        d = min(d, max_pos.x - p.x);
        d = min(d, max_pos.y - p.y);

        // Circle
        vec2 center = vec2(sin(time) * .4, 0.);
        d = min(d, length(p - center) - .15);

        return d;
    }

    void main()
    {
        // Render
        float dist = sd_colliders(v_uv);
        vec3 col = mix(
            vec3(.08, .6, .03),
            vec3(.005, .29, 0),
            remap01(dist, -.015 - px2uv, -.015)
        );
        col = mix(
            col,
            vec3(.3, .7, 1),
            remap01(dist, -px2uv, 0.)
        );

        // OETF
        col = pow(col, vec3(1. / 2.2));

        // Output
        out_col = vec4(col, 1);
    }
)glsl";

static const float plane_vertices[]{
    // vec2 pos
    -1.f, 1.f,  // Top-left
    1.f, 1.f,   // Top-right
    1.f, -1.f,  // Bottom-right
    -1.f, -1.f  // Bottom-left
};

static const GLuint plane_elements[] = {
    0, 1, 2, 2, 3, 0
};

// Boids

static constexpr math::vec2 boids_min_pos(-.9f, -.9f);
static constexpr math::vec2 boids_max_pos(.9f, .9f);
static constexpr float boids_speed = .6f;
static constexpr float boids_attention = .2f;
static constexpr float boids_attention_sqr = boids_attention * boids_attention;
static constexpr float boids_size = .03f;

static const char* boids_src_vert = R"glsl(
    #version 330 core

    in vec2 pos;
    in vec2 vel;

    out vec2 v_pos;
    out vec2 v_vel;

    void main()
    {
        v_pos = pos;
        v_vel = vel;
    }
)glsl";

static const char* boids_src_geo = R"glsl(
    #version 330 core

    layout(points) in;
    layout(triangle_strip, max_vertices = 6) out;

    uniform vec2 aspect;
    uniform float boids_size;

    in vec2 v_pos[];
    in vec2 v_vel[];

    out vec2 g_uv;

    #define PI 3.14159265358979

    mat2 rotate2D(float a) {
        float s = sin(a);
        float c = cos(a);
        return mat2(c, s, -s, c);
    }

    vec4 gen_vertex(vec2 offs, float angle)
    {
        vec2 p = v_pos[0] + rotate2D(angle) * offs;
        return vec4(p / aspect, 0, 1);
    }

    // Generate a square centered around v_pos and rotated based on v_vel
    void main()
    {
        // Calculate the rotation angle based on the velocity
        float angle = atan(v_vel[0].x, -v_vel[0].y) + PI;

        // Top left
        gl_Position = gen_vertex(vec2(-boids_size, boids_size), angle);
        g_uv = vec2(-1, 1);
        EmitVertex();

        // Top right
        gl_Position = gen_vertex(vec2(boids_size, boids_size), angle);
        g_uv = vec2(1, 1);
        EmitVertex();

        // Bottom left
        gl_Position = gen_vertex(vec2(-boids_size, -boids_size), angle);
        g_uv = vec2(-1, -1);
        EmitVertex();

        EndPrimitive();

        // Bottom left
        gl_Position = gen_vertex(vec2(-boids_size, -boids_size), angle);
        g_uv = vec2(-1, -1);
        EmitVertex();

        // Top right
        gl_Position = gen_vertex(vec2(boids_size, boids_size), angle);
        g_uv = vec2(1, 1);
        EmitVertex();

        // Bottom right
        gl_Position = gen_vertex(vec2(boids_size, -boids_size), angle);
        g_uv = vec2(1, -1);
        EmitVertex();

        EndPrimitive();
    }
)glsl";

static const char* boids_src_frag = R"glsl(
    #version 330 core

    uniform float px2uv;

    in vec2 g_uv;

    out vec4 out_col;

    float remap01(float inp, float inp_start, float inp_end)
    {
        return clamp((inp - inp_start) / (inp_end - inp_start), 0., 1.);
    }

    // https://www.shadertoy.com/view/clXBW4
    const int n_sides = 4;
    float sd_polygon(vec2 p, vec2[n_sides] v)
    {
        // Minimum distance
        float m = 1e9;

        // How many segments intersect a half line from p to (+inf, p.y)
        int num_intersections = 0;

        // The segments
        for (int i = 0; i < n_sides; i++)
        {
            // Line segment vertices
            vec2 a = v[i];
            vec2 b = v[(i + 1) % n_sides];

            // Slope and intercept
            float temp = b.x - a.x;
            if (temp == 0.) temp = 1e-9;
            float slope = (b.y - a.y) / temp;
            float intercept = a.y - slope * a.x;

            // Should we use the endpoints or the line?
            if (min(dot(p - a, b - a), dot(p - b, a - b)) < 0.)
            {
                // Distance from the endpoints
                m = min(m, min(distance(p, a), distance(p, b)));
            }
            else
            {
                // Distance from the line
                m = min(m, abs(slope * p.x - p.y + intercept) / sqrt(slope * slope + 1.));
            }

            // Intersect AB with half line from p to (+inf, p.y)
            // (only if p.y is between a.y and b.y)
            if (p.y > min(a.y, b.y) && p.y < max(a.y, b.y))
            {
                float x_where_y_equals_py = (p.y - intercept) / slope;
                if (x_where_y_equals_py > p.x)
                {
                    num_intersections++;
                }
            }
        }

        // Is num_intersections odd?
        if (num_intersections % 2 == 1) m = -m;

        return m;
    }

    // Signed distance from the edges of the boid shape
    float sd_boid(vec2 p)
    {
        const vec2[4] vertices = vec2[](
            vec2(-.9, -1),
            vec2(0, 1),
            vec2(.9, -1),
            vec2(0, -.5)
        );
        return sd_polygon(p, vertices);
    }

    void main()
    {
        // Render
        float dist = sd_boid(g_uv);
        vec3 col = mix(
            vec3(.017, .104, .33),
            vec3(.2, .616, 1),
            remap01(dist, -.32 + px2uv, -.32)
        );

        // OETF
        col = pow(col, vec3(1. / 2.2));
        
        // Output
        float alpha = remap01(dist, px2uv, 0.);
        out_col = vec4(col, alpha);
    }
)glsl";
