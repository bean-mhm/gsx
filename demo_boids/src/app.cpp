#include "app.h"

// STD
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <cstdint>

static const char* src_vert = R"glsl(
#version 330 core
precision highp float;

// Uniforms
uniform vec2 aspect;

// Inputs
in vec2 pos;

// Outputs
out vec2 v_uv;

void main()
{
    v_uv = pos * aspect;
    gl_Position = vec4(pos, 0.0, 1.0);
}
)glsl";

static const char* src_frag = R"glsl(
#version 330 core
precision highp float;

// Uniforms
uniform float px2uv;
uniform float time;

// Inputs
in vec2 v_uv;

// Outputs
out vec4 out_col;

// Remap to [0, 1] (clamped)
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
        vec3(.2, .6, 1.),
        vec3(.2, .8, .1),
        remap01(sd_colliders(v_uv), px2uv, 0.)
    );
    
    // OETF
    col = pow(col, vec3(1. / 2.2));
    
    // Output
    out_col = vec4(col, 1.);
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

static void glfw_error_callback(int error, const char* description);
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void app_t::run()
{
    init();
    main_loop();
    cleanup();
}

void app_t::init()
{
    init_context();
    init_rendering();
}

void app_t::main_loop()
{
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render
        render();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();
    }
}

void app_t::cleanup()
{
    // OpenGL objects
    {
        glDeleteProgram(plane_shader_program);
        glDeleteShader(plane_frag_shader);
        glDeleteShader(plane_vert_shader);

        glDeleteBuffers(1, &plane_ebo);
        glDeleteBuffers(1, &plane_vbo);

        glDeleteVertexArrays(1, &plane_vao);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
}

void app_t::init_context()
{
    // Initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        throw std::runtime_error("Failed to initialize GLFW.");
    }

    // Window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // Create window with graphics context
    window = glfwCreateWindow(initial_width, initial_height, initial_title, nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to create a window.");
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);

    // Enable VSync
    glfwSwapInterval(1);

    // Key callback
    glfwSetKeyCallback(window, glfw_key_callback);

    // Initialize GLEW for loading OpenGL extensions
    glewExperimental = GL_TRUE;
    GLenum glew_result = glewInit();
    if (glew_result != GLEW_OK)
    {
        cleanup();
        throw std::runtime_error("Failed to initialize GLEW: " + std::to_string(glew_result));
    }
}

void app_t::init_rendering()
{
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
    make_shader(plane_vert_shader, "plane vertex shader", GL_VERTEX_SHADER, src_vert);
    make_shader(plane_frag_shader, "plane fragment shader", GL_FRAGMENT_SHADER, src_frag);

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
}

void app_t::render()
{
    // Time
    float time = glfwGetTime();

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

    // Bind the plane VAO
    glBindVertexArray(plane_vao);

    // Bind the plane EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_ebo);

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
        glUniform1f(location, time);
    }

    // Draw the plane
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

static void glfw_error_callback(int error, const char* description)
{
    std::cerr << "GLFW error " << error << ": " << description << '\n';
}

static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
