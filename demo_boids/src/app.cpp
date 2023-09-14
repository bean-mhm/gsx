#include "app.h"

// STD
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <cstdint>

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

void app_t::run()
{
    init_context();
    init_rendering();
    main_loop();
    cleanup();
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
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    // Create window with graphics context
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
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
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK)
    {
        cleanup();
        throw std::runtime_error("Failed to initialize GLEW: " + std::to_string(glewInitResult));
    }
}

void app_t::init_rendering()
{
    //
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

void app_t::render()
{
    // Render dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    float aspect = width / (float)height;

    // Time
    float t = glfwGetTime();

    // Calculate the clear color
    static constexpr float freq = .5f;
    glm::vec3 col(2.f * glm::pi<float>() * freq * t);
    col += 2.f * glm::pi<float>() * glm::vec3(0, .333f, .667f);
    col = glm::cos(col) * .5f + .5f;
    col = glm::pow(col, glm::vec3(1.f / 2.2f));

    // Clear the screen
    glClearColor(col.r, col.g, col.b, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
}
