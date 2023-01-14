#include <fmt/format.h>

#include <glbinding/gl/gl.h>
#include <glbinding/glbinding.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <imgui.h>

using namespace gl;

GLFWwindow *g_window;

static void glfw_error_callback(int error, const char *description) {
    fmt::print(stderr, "glfw Error {}: {}\n", error, description);
}

void init_glfw() {
    glfwInit();
    glfwSetErrorCallback(glfw_error_callback);
    g_window = glfwCreateWindow(640, 480, "raymarch", nullptr, nullptr);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
    glfwMakeContextCurrent(g_window);
    glbinding::initialize(glfwGetProcAddress);
    glfwSwapInterval(1);
}

void init_gui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(g_window, true);
    ImGui_ImplOpenGL3_Init("#version 460");
}

void deinit_gui() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    init_glfw();
    glfwMakeContextCurrent(g_window);
    init_gui();
    while (!glfwWindowShouldClose(g_window)) {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGui::ShowDemoWindow();
        ImGui::Render();

        int display_w, display_h;
        glClearColor(1.f, 1.f, 1.f, 1.00f);
        glClear(GL_COLOR_BUFFER_BIT);
        glfwGetFramebufferSize(g_window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

//        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(g_window);
    }
    deinit_gui();
    glfwTerminate();
}
