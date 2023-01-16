#ifndef RAYMARCH_SCENE_HPP
#define RAYMARCH_SCENE_HPP

class Scene {
public:
    Scene() = default;
    ~Scene();
    void init();
    void draw() const;
private:
    unsigned int m_vao;
    unsigned int m_shader;
};

#endif//RAYMARCH_SCENE_HPP
