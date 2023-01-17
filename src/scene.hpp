#ifndef RAYMARCH_SCENE_HPP
#define RAYMARCH_SCENE_HPP

#include "shader.hpp"

class Scene {
public:
    Scene() = default;
    ~Scene();
    void init();
    void draw();
private:
    unsigned int m_vao;
    Shader m_shader;
};

#endif//RAYMARCH_SCENE_HPP
