#ifndef RAYMARCH_SHADER_HPP
#define RAYMARCH_SHADER_HPP

#include <map>

#include <glbinding/gl/gl.h>
#include <glm/glm.hpp>
using namespace gl;

class Shader {
public:
    unsigned int id;
    void init(const char *vertex_source, const char *fragment_source);
    ~Shader();
    void use() const;
    template<typename F, typename... T>
    void set(const char *name, F func, T... args) {
        auto it = m_uniforms.find(name);
        if (it == m_uniforms.end()) {
            m_uniforms[name] = glGetUniformLocation(id, name);
        }
        func(m_uniforms[name], args...);
    }
    void set_bool(const char *name, bool value) const;
    void set_int(const char *name, int value) const;
    void set_float(const char *name, float value) const;
    void set_vec3(const char *name, glm::vec3 v) const;
    void set_vec4(const char *name, glm::vec4 v) const;

private:
    std::map<const char *, int> m_uniforms;
};


#endif//RAYMARCH_SHADER_HPP
