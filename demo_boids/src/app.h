#pragma once

// OpenGL
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "GL/glew.h"
#include "GLFW/glfw3.h"

// GLM
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class app_t
{
public:
    void run();

private:
    GLFWwindow* window;

    void init_context();
    void init_rendering();
    void main_loop();
    void cleanup();

    void render();

};
