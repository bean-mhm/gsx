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

uniform vec2 resolution;

in vec2 pos;
in vec2 uv01;

out vec2 v_coord;

void main()
{
    v_coord = uv01 * resolution;
    gl_Position = vec4(pos, 0.0, 1.0);
}
)glsl";

static const char* src_frag = R"glsl(
#version 330 core

precision highp float;

uniform vec2 resolution;

in vec2 v_coord;

out vec4 out_col;

vec2 screen_to_uv(vec2 coord)
{
    return (2. * coord - resolution) / min(resolution.x, resolution.y);
}

void main()
{
    // UV
    vec2 uv = screen_to_uv(v_coord);
    
    // Render
    vec3 col = vec3(uv, .02);
    
    // OETF
    col = pow(col, vec3(1. / 2.2));
    
    // Output
    out_col = vec4(col, 1.);
}
)glsl";

static const float plane_vertices[]{
    // vec2 pos, vec2 uv
    -1.f, 1.f, 0.f, 1.f,   // Top-left
    1.f, 1.f, 1.f, 1.f,    // Top-right
    1.f, -1.f, 1.f, 0.f,   // Bottom-right
    -1.f, -1.f, 0.f, 0.f,  // Bottom-left
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
            4 * sizeof(float), 0);
    }
    {
        GLint location = glGetAttribLocation(plane_shader_program, "uv01");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE,
            4 * sizeof(float), (void*)(2 * sizeof(float)));
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
    glClearColor(0.f, 0.f, 0.f, 0.f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Bind the plane shader program
    glUseProgram(plane_shader_program);

    // Bind the plane VAO
    glBindVertexArray(plane_vao);

    // Bind the plane EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, plane_ebo);

    // Plane uniforms
    {
        GLint location = glGetUniformLocation(plane_shader_program, "resolution");
        glUniform2f(location, width, height);
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
