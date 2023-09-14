#include "app.h"

// STD
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <cstdint>

static const char* src_vert = R"glsl(
    #version 330 core
    
    uniform vec2 offs;
    
    in vec2 pos;
    in vec3 col;
    
    out vec3 v_col;
    
    void main()
    {
        v_col = col;
        gl_Position = vec4(pos + offs, 0.0, 1.0);
    }
)glsl";

static const char* src_frag = R"glsl(
    #version 330 core
    
    in vec3 v_col;
    
    out vec4 out_col;
    
    void main()
    {
        vec3 col = pow(v_col, vec3(1. / 2.2));
        out_col = vec4(col, 1.);
    }
)glsl";

static const float vertices[]{
    // vec2 pos, vec3 col
    0.0f, 0.5f, 1.0f, 0.0f, 0.0f,   // Vertex 1: Red
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // Vertex 2: Green
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f  // Vertex 3: Blue
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
    // VAO
    glGenVertexArrays(1, &vao);

    // VBO
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Shaders
    make_shader(vert_shader, "vertex shader", GL_VERTEX_SHADER, src_vert);
    make_shader(frag_shader, "fragment shader", GL_FRAGMENT_SHADER, src_frag);

    // Program
    shader_program = glCreateProgram();
    glAttachShader(shader_program, vert_shader);
    glAttachShader(shader_program, frag_shader);
    glBindFragDataLocation(shader_program, 0, "out_col");
    glLinkProgram(shader_program);

    // Vertex attributes
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    {
        GLint location = glGetAttribLocation(shader_program, "pos");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
    }
    {
        GLint location = glGetAttribLocation(shader_program, "col");
        glEnableVertexAttribArray(location);
        glVertexAttribPointer(location, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
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

    // Activate the shader program
    glUseProgram(shader_program);

    // Activate the VAO
    glBindVertexArray(vao);

    // Uniforms
    {
        GLint location = glGetUniformLocation(shader_program, "offs");
        glUniform2f(location, .2f * cos(time * 2.f), .2f * sin(time * 2.f));
    }

    // Draw
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
