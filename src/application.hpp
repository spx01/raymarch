#ifndef RAYMARCH_APPLICATION_HPP
#define RAYMARCH_APPLICATION_HPP

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "app_gui.hpp"
#include "scene.hpp"

class Application {
public:
    static Application *instance();
    void run();

private:
    Application();
    ~Application();
    GLFWwindow *m_window;
    AppGUI m_gui;
    Scene m_scene;
    int m_width;
    int m_height;

    static void on_error(int error, const char *description);
    static void on_key(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void on_resize(GLFWwindow *window, int width, int height);
    static void on_mouse_move(GLFWwindow *window, double xpos, double ypos);
    static void on_scroll(GLFWwindow *window, double xoffset, double yoffset);
    static void on_mouse_button(GLFWwindow *window, int button, int action, int mods);
};


#endif//RAYMARCH_APPLICATION_HPP
