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

void Scene::init_shader(const char *path) {
    glGenVertexArrays(1, &m_vao);
    glBindVertexArray(m_vao);
    float vertices[] = {
            1.0f, 1.0f,
            1.0, -1.0f,
            -1.0f, -1.0f,
            -1.0f, 1.0f};
    unsigned int indices[] = {0, 1, 2, 0, 2, 3};
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

    std::ifstream file(path);
    std::string frag_source{std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
    m_shader.init(R"(
#version 330 core
layout (location = 0) in vec2 position;
smooth out vec2 ray_offset;
void main() {
    gl_Position = vec4(position, 0.0, 1.0);
    ray_offset = position;
})",
                  frag_source.c_str());

    m_dirty = true;
}

Scene::Scene() {
    m_last_x = 0.0f;
    m_last_y = 0.0f;
    m_mouse_down = false;
    obj_pos = vec3(0.0f, 0.0f, 1.0f);
    obj_rot = identity<quat>();
    obj_params = vec4(0.5f, 0.5f, 0.5f, 0.0f);
    obj_color = vec3(1.0f, 1.0f, 0.0f);
    obj_type = 2;
    camera_pos = vec3(0.0f, 0.0f, -1.0f);
    camera_dir = vec3(0.0f, 0.0f, 1.0f);
    max_dist = 100.0f;
    light_pos = vec3(0.0f, 0.5f, -3.0f);
    light_color = vec3(1.0f, 1.0f, 1.0f);
    light_intensity = 1.0f;
    ambient_color = vec3(1.0f, 1.0f, 1.0f);
    ambient_intensity = 0.05f;
    fog_color = vec3(0.1f, 0.21f, 0.9f);
    m_dirty = false;
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

void Scene::update_uniforms() {
    if (!m_dirty) {
        return;
    }
    m_dirty = false;
    m_shader.use();
    auto global_to_local = translate(mat4_cast(conjugate(obj_rot)), -obj_pos);
    auto local_to_global = inverse(global_to_local);
    m_shader.set("global_to_local", glUniformMatrix4fv, 1, GL_FALSE, value_ptr(global_to_local));
    m_shader.set("local_to_global", glUniformMatrix4fv, 1, GL_FALSE, value_ptr(local_to_global));
    m_shader.set("obj_params", glUniform4fv, 1, value_ptr(obj_params));
    m_shader.set("obj_color", glUniform3fv, 1, value_ptr(obj_color));
    m_shader.set<void(GLint, GLint)>("obj_type", glUniform1i, 2);
    m_shader.set("camera_pos", glUniform3fv, 1, value_ptr(camera_pos));
    m_shader.set("camera_dir", glUniform3fv, 1, value_ptr(camera_dir));
    m_shader.set("light_pos", glUniform3fv, 1, value_ptr(light_pos));
    m_shader.set("light_color", glUniform3fv, 1, value_ptr(light_color));
    m_shader.set("light_intensity", glUniform1f, light_intensity);
    m_shader.set("ambient_color", glUniform3fv, 1, value_ptr(ambient_color));
    m_shader.set("ambient_intensity", glUniform1f, ambient_intensity);
    m_shader.set("fog_color", glUniform3fv, 1, value_ptr(fog_color));
    m_shader.set("max_dist", glUniform1f, max_dist);
}

void Scene::on_mouse_move(double x, double y) {
    if (m_mouse_down) {
        auto dx = x - m_last_x;
        auto dy = y - m_last_y;
        float pitch = 10 * sin(dy);
        float yaw = 10 * sin(dx);
        obj_rot = quat(vec3(pitch, yaw, 0.0f)) * obj_rot;
        m_dirty = true;
    }
    m_last_x = x;
    m_last_y = y;
}

void Scene::on_mouse_button(MButton button, Action action) {
    if (button == MButton::LEFT) {
        m_mouse_down = action == Action::PRESS;
    }
}
