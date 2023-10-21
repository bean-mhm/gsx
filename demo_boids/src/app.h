#pragma once

// Internal
#include "gl_utils.h"

class app_t
{
public:
    void run();

private:
    static constexpr const char* initial_title = "Boids";
    static constexpr int initial_width = 960;
    static constexpr int initial_height = 720;

    GLFWwindow* window;

    void init_context();
    void cleanup_context();

};
