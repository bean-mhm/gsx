#include "gl_utils.h"

#include <iostream>

void make_shader(
    GLuint& shader,
    const std::string& name,
    GLenum type,
    const char* source
)
{
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    char buffer[512];
    glGetShaderInfoLog(shader, 512, nullptr, buffer);
    std::string info_log = buffer;

    std::string message =
        (status == GL_TRUE)
        ? "\"" + name + "\" was compiled successfully"
        : "failed to compile \"" + name + "\"";

    if (info_log.empty())
    {
        message += '.';
    }
    else
    {
        message += ": \"" + info_log + "\"";
    }

    std::cout << message << '\n';
}
