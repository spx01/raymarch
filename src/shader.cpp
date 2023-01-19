#include "shader.hpp"

#include <fstream>

#include <glbinding/gl/gl.h>
using namespace gl;
#include <fmt/format.h>

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

void Shader::init(const char *vertex_source, const char *fragment_source) {
    id = glCreateProgram();
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    compile_shader(vertex_shader, vertex_source, "vertex shader");
    compile_shader(fragment_shader, fragment_source, "fragment shader");
    glAttachShader(id, vertex_shader);
    glAttachShader(id, fragment_shader);
    glLinkProgram(id);
    int status;
    glGetProgramiv(id, GL_LINK_STATUS, &status);
    if (!status) {
        char info_log[512];
        glGetProgramInfoLog(id, 512, nullptr, info_log);
        fmt::print("linking: {}\n", info_log);
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

Shader::~Shader() {
    glDeleteProgram(id);
}

void Shader::use() const {
    glUseProgram(id);
}
