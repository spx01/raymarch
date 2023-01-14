#include "application.hpp"

#include <fmt/format.h>
#include <glbinding/glbinding.h>

#include "common.hpp"

Application::Application() {
    glfwSetErrorCallback(on_error);
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    m_window = glfwCreateWindow(640, 480, "raymarch", nullptr, nullptr);
    if (!m_window) {
        throw std::runtime_error("Failed to create window");
    }
    glfwMakeContextCurrent(m_window);
    glbinding::initialize(glfwGetProcAddress);
    glfwSwapInterval(1);
    glfwSetKeyCallback(m_window, on_key);
    m_gui.init(m_window);
}
Application::~Application() {
    glfwDestroyWindow(m_window);
    glfwTerminate();
}
Application *Application::instance() {
    static Application app;
    return &app;
}
void Application::run() const {
    while (!glfwWindowShouldClose(m_window)) {
        m_gui.draw();
        glClear(GL_COLOR_BUFFER_BIT);
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
