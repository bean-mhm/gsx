#include "app.h"

// STD
#include <iostream>
#include <memory>
#include <stdexcept>

// GSX
#include "gsx/gsx.h"

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
    ecs::world_t world(
        "Boids",
        ecs::log_level_t::info,
        std::make_shared<ecs::ostream_logger_t>(std::cout)
    );

    // Create a PRNG
    math::prng_t prng;

    // Add boid attractors
    std::vector<attractor_t> attractors;
    {
        // The first attractor will be rotating around the origin by the attractors system
        attractor_t attractor;
        attractor.pos = math::vec2(0);
        attractor.strength = 1.5;
        attractors.push_back(attractor);
    }
    {
        // This one will move the boids away from the bottom
        attractor_t attractor;
        attractor.pos = math::vec2(0, boid_min_pos.y);
        attractor.strength = -.8;
        attractors.push_back(attractor);
    }

    // Add boids
    spatial::grid_2d_t<boid_t> boids(
        math::bounds2(boid_min_pos, boid_max_pos),
        math::ivec2(6)
    );
    for (usize i = 0; i < 200; i++)
    {
        boid_t boid;

        boid.pos = math::vec2(
            prng.next_f32(boid_min_pos.x, boid_max_pos.x),
            prng.next_f32(boid_min_pos.y, boid_max_pos.y)
        );

        f32 angle = prng.next_f32(0, math::tau);
        boid.vel = boid_speed * math::vec2(math::cos(angle), math::sin(angle));

        boids.insert(boid);
    }

    // Add systems
    {
        world.add_system(std::make_shared<attractor_system_t>(
            "Attractor", 0, false, attractors
        ));

        world.add_system(std::make_shared<boid_system_t>(
            "Boid", 1, false, boids, attractors
        ));

        world.add_system(std::make_shared<render_system_t>(
            "Render", 2, true, window, boids
        ));
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
