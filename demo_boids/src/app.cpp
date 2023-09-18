#include "app.h"

// STD
#include <iostream>
#include <string>
#include <stdexcept>
#include <cmath>
#include <cstdint>

// TEF
#include "tef/tef.h"

// Internal
#include "constants.h"
#include "components.h"
#include "systems.h"

static void glfw_error_callback(int error, const char* description);
static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

void app_t::run()
{
    // Initialize a window with an OpenGL context
    init_context();

    // Create a world
    tef::world_t world(
        "Boids",
        tef::log_level_t::verbose,
        std::make_shared<tef::ostream_logger_t>(std::cout)
    );

    // Add entities
    for (size_t i = 0; i < 20; i++)
    {
        // Add a boid component
        c_boid boid;
        boid.pos = glm::vec2(
            world.prng.next_float(boids_min_pos.x, boids_max_pos.x),
            world.prng.next_float(boids_min_pos.y, boids_max_pos.y)
        );
        boid.vel = glm::vec2(.05f, 0.02f);
        world.add_component_of_type<c_boid>(boid);
    }

    // Add systems
    {
        world.add_system(std::make_shared<s_boids>("Boids", 0));
        world.add_system(std::make_shared<s_rendering>("Rendering", 1, window));
    }

    // Run
    world.run();

    // Clean up the OpenGL context
    cleanup_context();
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
        cleanup_context();
        throw std::runtime_error("Failed to initialize GLEW: " + std::to_string(glew_result));
    }
}

void app_t::cleanup_context()
{
    glfwDestroyWindow(window);
    glfwTerminate();
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
