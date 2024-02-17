#include "app.h"

#include <iostream>
#include <memory>
#include <stdexcept>

#include "gsx/gsx.h"

#include "constants.h"
#include "components.h"
#include "systems.h"

static void glfw_error_callback(int error, const char* description);
static void glfw_key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
);

void app_t::run()
{
    init_context();

    ecs::world_t world(
        "Boids",
        ecs::log_level_t::info,
        std::make_shared<ecs::ostream_logger_t>(std::cout)
    );

    math::prng_t prng;

    std::vector<attractor_t> attractors;
    {
        // the first attractor will be rotating around the origin by the
        // attractors system
        attractor_t attractor;
        attractor.pos = math::vec2(0);
        attractor.strength = 1.5;
        attractors.push_back(attractor);
    }
    {
        // this one will move the boids away from the bottom
        attractor_t attractor;
        attractor.pos = math::vec2(0, boid_min_pos.y);
        attractor.strength = -.8;
        attractors.push_back(attractor);
    }

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


    world.add_system(std::make_shared<attractor_system_t>(
        "attractor", ecs::execution_scheme_t(0), attractors
    ));

    world.add_system(std::make_shared<boid_system_t>(
        "boid", ecs::execution_scheme_t(1), boids, attractors
    ));

    world.add_system(std::make_shared<render_system_t>(
        "render", ecs::execution_scheme_t(2, true), window, boids
    ));

    world.run();

    cleanup_context();
}

void app_t::init_context()
{
    // initialize GLFW
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        throw std::runtime_error("failed to initialize GLFW");
    }

    // window hints
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // create window with graphics context
    window = glfwCreateWindow(
        initial_width,
        initial_height,
        initial_title,
        nullptr,
        nullptr
    );
    if (!window)
    {
        glfwTerminate();
        throw std::runtime_error("failed to create a window");
    }

    // make the window's context current
    glfwMakeContextCurrent(window);

    // enable VSync
    glfwSwapInterval(1);

    // key callback
    glfwSetKeyCallback(window, glfw_key_callback);

    // initialize GLEW for loading OpenGL extensions
    glewExperimental = GL_TRUE;
    GLenum glew_result = glewInit();
    if (glew_result != GLEW_OK)
    {
        cleanup_context();
        throw std::runtime_error(
            "failed to initialize GLEW: " + std::to_string(glew_result)
        );
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

static void glfw_key_callback(
    GLFWwindow* window,
    int key,
    int scancode,
    int action,
    int mods
)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
