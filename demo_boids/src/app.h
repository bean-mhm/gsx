#pragma once

struct GLFWwindow;

class app_t
{
public:
    void run();

private:
    GLFWwindow* window;

    void init();
    void main_loop();
    void cleanup();

};
