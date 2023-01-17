#include "scene.hpp"

#include <fstream>

#include <glbinding/gl/gl.h>
using namespace gl;
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
using namespace glm;
#include <fmt/format.h>

void Scene::init() {
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

    std::ifstream file("assets/shader_rewrite.glsl.gen");
    std::string frag_source{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    m_shader.init(R"(
#version 460 core
layout (location = 0) in vec2 position;
smooth out vec2 ray_offset;
void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    ray_offset = position;
})", frag_source.c_str());

    vec3 obj_pos = vec3(0.0f, 0.0f, 1.0f);
    auto q = identity<quat>();
    q *= angleAxis(radians(-30.0f), vec3(1.0f, 0.0f, 0.0f));
    q *= angleAxis(radians(45.0f), vec3(0.0f, 1.0f, 0.0f));

    auto global_to_local = translate(mat4_cast(conjugate(q)), -obj_pos);
    auto local_to_global = inverse(global_to_local);

    m_shader.use();
    m_shader.set("global_to_local", glUniformMatrix4fv, 1, GL_FALSE, value_ptr(global_to_local));
    m_shader.set("local_to_global", glUniformMatrix4fv, 1, GL_FALSE, value_ptr(local_to_global));
    m_shader.set("obj_params", glUniform4f, 0.5f, 0.5f, 0.5f, 0.02f);
    m_shader.set("obj_color", glUniform3f, 1.0f, 1.0f, 0.1f);
    m_shader.set<void(GLint, GLint)>("obj_type", glUniform1i, 2);
    m_shader.set("camera_pos", glUniform3f, 0.0f, 0.0f, -1.0f);
    m_shader.set("camera_dir", glUniform3f, 0.0f, 0.0f, 1.0f);
    m_shader.set("light_pos", glUniform3f, 0.0f, 0.5f, -3.0f);
    m_shader.set("light_color", glUniform3f, 1.0f, 1.0f, 1.0f);
    m_shader.set("light_intensity", glUniform1f, 1.0f);
    m_shader.set("ambient_color", glUniform3f, 1.0f, 1.0f, 1.0f);
    m_shader.set("ambient_intensity", glUniform1f, 0.05f);
    m_shader.set("fog_color", glUniform3f, 0.0f, 0.0f, 1.0f);
    m_shader.set<void(GLint, GLint)>("max_steps", glUniform1i, 500);
}

Scene::~Scene() {
    glDeleteVertexArrays(1, &m_vao);
}

void Scene::draw() {
    m_shader.use();
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    glBindVertexArray(0);
    glUseProgram(0);
}
