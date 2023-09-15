#pragma once

// OpenGL utils
#include "gl_utils.h"

// GLM
#include "glm/glm.hpp"
#include "glm/ext.hpp"

class app_t
{
public:
    void run();

private:
    static constexpr const char* initial_title = "Boids";
    static constexpr int initial_width = 800;
    static constexpr int initial_height = 600;

    GLFWwindow* window;

    GLuint plane_vao = 0;
    GLuint plane_vbo = 0;
    GLuint plane_ebo = 0;
    GLuint plane_vert_shader = 0;
    GLuint plane_frag_shader = 0;
    GLuint plane_shader_program = 0;

    void init();
    void main_loop();
    void cleanup();

    void init_context();
    void init_rendering();
    void render();

};