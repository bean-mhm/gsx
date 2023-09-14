#pragma once

//STD
#include <string>

// OpenGL
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include "GL/glew.h"
#include "GLFW/glfw3.h"

void make_shader(GLuint& shader, const std::string& name, GLenum type, const char* source);
