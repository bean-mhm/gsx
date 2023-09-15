#include "app.h"

// STD
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <cstdint>

// Internal
#include "constants.h"

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
    glBindBuffer(GL_ARRAY_BUFFER, boids_vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(boids_points), boids_points, GL_DYNAMIC_DRAW);

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
            4 * sizeof(float), 0);
    }
    {
        GLint location = glGetAttribLocation(boids_shader_program, "vel");
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
        glUniform1f(location, time);
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
        GLint location = glGetUniformLocation(plane_shader_program, "aspect");
        glUniform2f(location, (float)width / std::min(width, height), (float)height / std::min(width, height));
    }

    // Bind the boids VAO
    glBindVertexArray(boids_vao);

    // Draw the boids
    glDrawArrays(GL_POINTS, 0, (sizeof(boids_points) / sizeof(boids_points[0])) / 4);
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
