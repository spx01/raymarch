#include "application.hpp"

#include <fmt/format.h>
#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
using namespace gl;
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

Application::Application() {
    glfwSetErrorCallback(on_error);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwWindowHint(GLFW_SAMPLES, 4);
    m_window = glfwCreateWindow(800, 800, "raymarch", nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(m_window);
    glbinding::initialize(glfwGetProcAddress);
    glfwSwapInterval(1);
    glEnable(GL_MULTISAMPLE);

    glfwSetKeyCallback(m_window, on_key);
    glfwSetWindowSizeCallback(m_window, on_resize);
    glfwSetCursorPosCallback(m_window, on_mouse_move);
    glfwSetScrollCallback(m_window, on_scroll);
    glfwSetMouseButtonCallback(m_window, on_mouse_button);
    m_gui.init(m_window);
    m_scene.init();
}
Application::~Application() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
Application *Application::instance() {
    static Application app;
    return &app;
}
void Application::run() {
    while (!glfwWindowShouldClose(m_window)) {
        m_gui.draw();
        glClear(GL_COLOR_BUFFER_BIT);
        m_scene.draw();
        m_gui.render();
        glfwSwapBuffers(m_window);
        glfwPollEvents();
    }
}
void Application::on_error(int error, const char *description) {
    fmt::print("GLFW error {}: {}\n", error, description);
}

void Application::on_key(GLFWwindow *window, int key, int scancode, int action, int mods) {
}

void Application::on_resize(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::on_mouse_move(GLFWwindow *window, double xpos, double ypos) {
}

void Application::on_scroll(GLFWwindow *window, double xoffset, double yoffset) {
}

void Application::on_mouse_button(GLFWwindow *window, int button, int action, int mods) {
}
