#include "scene.hpp"

#include <fmt/format.h>

#include "common.hpp"

void compile_shader(unsigned int shader, const char *source, const char *label) {
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char info_log[512];
        glGetShaderInfoLog(shader, 512, nullptr, info_log);
        fmt::print("{}: {}\n", label, info_log);
    }
}

void Scene::init() {
    m_shader = glCreateProgram();
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(vertex_shader, R"(
#version 460 core
layout (location = 0) in vec2 position;
out vec4 vertex_color;
void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    vertex_color = vec4(position.xy, 0.0, 1.0);
})", "vertex shader");
    compile_shader(fragment_shader, R"(
#version 460 core
out vec4 color;
in vec4 vertex_color;
void main() {
    color = vertex_color;
})", "fragment shader");
    glAttachShader(m_shader, vertex_shader);
    glAttachShader(m_shader, fragment_shader);
    glLinkProgram(m_shader);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    float vertices[] = {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
Scene::~Scene() {
    glDeleteShader(m_shader);
    glDeleteVertexArrays(1, &m_vao);
}
void Scene::draw() const {
    glUseProgram(m_shader);
    glBindVertexArray(m_vao);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glUseProgram(0);
}
