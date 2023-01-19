#ifndef RAYMARCH_SCENE_HPP
#define RAYMARCH_SCENE_HPP

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

#include "app_common.hpp"
#include "shader.hpp"

class Scene {
public:
    Scene();
    ~Scene();
    void init_shader(const char *path);
    void draw();
    void update_uniforms();
    void on_key(int key, int scancode, int action, int mods) {}
    void on_mouse_move(double x, double y);
    void on_scroll(double x, double y) {}
    void on_mouse_button(MButton button, Action action);

    vec3 obj_pos;
    quat obj_rot;
    vec4 obj_params;
    vec3 obj_color;
    int obj_type;
    vec3 camera_pos;
    vec3 camera_dir;
    vec3 light_pos;
    vec3 light_color;
    float light_intensity;
    vec3 ambient_color;
    float ambient_intensity;
    vec3 fog_color;
    float max_dist;

private:
    unsigned int m_vao;
    Shader m_shader;
    float m_last_x;
    float m_last_y;
    bool m_mouse_down;
    bool m_dirty;
};

#endif//RAYMARCH_SCENE_HPP
