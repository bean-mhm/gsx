#pragma once

// Internal
#include "gl_utils.h"

// Plane

static const char* plane_src_vert = R"glsl(
    #version 330 core
    precision highp float;
    
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
    precision highp float;
    
    uniform float px2uv;
    uniform float time;
    
    in vec2 v_uv;
    
    out vec4 out_col;
    
    float remap01(float inp, float inp_start, float inp_end)
    {
        return clamp((inp - inp_start) / (inp_end - inp_start), 0., 1.);
    }
    
    // Signed distance from the colliders
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
        vec3 col = mix(
            vec3(.3, .7, 1),
            vec3(.08, .6, .03),
            remap01(sd_colliders(v_uv), px2uv, 0.)
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

static const char* boids_src_vert = R"glsl(
    #version 330 core
    precision highp float;
    
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
    precision highp float;
    
    layout(points) in;
    layout(triangle_strip, max_vertices = 6) out;
    
    uniform vec2 aspect;
    
    in vec2 v_pos[];
    in vec2 v_vel[];
    
    out vec2 g_uv;
    
    #define PI 3.14159265358979
    
    vec2 rotate(vec2 v, float a) {
        float s = sin(a);
        float c = cos(a);
        mat2 m = mat2(c, s, -s, c);
        return m * v;
    }
    
    vec4 gen_vertex(vec2 offs, float angle)
    {
        vec2 p = rotate(v_pos[0] + offs, angle);
        return vec4(p / aspect, 0, 1);
    }
    
    // Generate a square centered around v_pos and rotated based on v_vel
    void main()
    {
        // Square radius (boid size)
        const float radius = .1;
        
        // Calculate the rotation angle based on the velocity
        vec2 dir = normalize(v_vel[0]);
        float angle = atan(-dir.y, -dir.x) + PI;
        
        // Top left
        gl_Position = gen_vertex(vec2(-radius, radius), angle);
        g_uv = vec2(-1, 1);
        EmitVertex();
        
        // Top right
        gl_Position = gen_vertex(vec2(radius, radius), angle);
        g_uv = vec2(1, 1);
        EmitVertex();
        
        // Bottom left
        gl_Position = gen_vertex(vec2(-radius, -radius), angle);
        g_uv = vec2(-1, -1);
        EmitVertex();
        
        EndPrimitive();
        
        // Bottom left
        gl_Position = gen_vertex(vec2(-radius, -radius), angle);
        g_uv = vec2(-1, -1);
        EmitVertex();
        
        // Top right
        gl_Position = gen_vertex(vec2(radius, radius), angle);
        g_uv = vec2(1, 1);
        EmitVertex();
        
        // Bottom right
        gl_Position = gen_vertex(vec2(radius, -radius), angle);
        g_uv = vec2(1, -1);
        EmitVertex();
        
        EndPrimitive();
    }
)glsl";

static const char* boids_src_frag = R"glsl(
    #version 330 core
    precision highp float;
    
    in vec2 g_uv;
    
    out vec4 out_col;
    
    void main()
    {
        // Render
        vec3 col = vec3(g_uv, 0);
        
        // OETF
        col = pow(col, vec3(1. / 2.2));
        
        // Output
        out_col = vec4(col, 1);
    }
)glsl";

static const float boids_points[]{
    // vec2 pos, vec2 vel
    0.f, 0.f, 1.f, 1.f,
    .6f, 0.f, .1f, 1.f,
    -.5f, 0.f, -.4f, 0.f,
    .3f, -.2f, .2f, -.4f
};
