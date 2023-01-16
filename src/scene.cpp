#include "scene.hpp"

#include <fstream>
#include <iostream>

#include <fmt/format.h>
#include <glbinding/gl/gl.h>
using namespace gl;

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
smooth out vec2 ray_offset;
void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    ray_offset = position;
})", "vertex shader");
    std::ifstream file("assets/shader.frag");
    std::string frag_source{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    compile_shader(fragment_shader, frag_source.c_str(), "fragment shader");
    glAttachShader(m_shader, vertex_shader);
    glAttachShader(m_shader, fragment_shader);
    glLinkProgram(m_shader);
    // error check
    int status;
    glGetProgramiv(m_shader, GL_LINK_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetProgramInfoLog(m_shader, 512, nullptr, info_log);
        fmt::print("shader: {}\n", info_log);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    float vertices[] = {
            1.0f, 1.0f,
            1.0, -1.0f,
            -1.0f, -1.0f,
            -1.0f, 1.0f
    };
    unsigned int indices[] = {
            0, 1, 2,
            0, 2, 3,
    };
    unsigned int vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    unsigned int ebo;
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
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
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}
