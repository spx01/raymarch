#ifndef RAYMARCH_APPLICATION_HPP
#define RAYMARCH_APPLICATION_HPP

#include <GLFW/glfw3.h>

class Application {
public:
    static Application *instance();
    void run();

private:
    GLFWwindow *m_window;
};


#endif//RAYMARCH_APPLICATION_HPP
