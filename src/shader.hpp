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

private:
    std::map<const char *, int> m_uniforms;
};


#endif//RAYMARCH_SHADER_HPP
