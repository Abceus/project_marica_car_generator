#pragma once
#include "render_system/fragment_shader.h"
#include "render_system/vertex_shader.h"
#include <memory>
#include <string>
#include <mul_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <type_traits>


class ShaderProgram {
public:
    void init(const std::string& vertexShaderText,
              const std::string& fragmentShaderText);
    void destroy();

    void bind() const;
    void unbind() const;

    bool isValid() const;

    template <typename T>
    void setUniform(const std::string& name, const T& value);

private:
    static constexpr GLuint INVALID_PROGRAM_INDEX = 0;
    GLuint programIndex = INVALID_PROGRAM_INDEX;

    std::unique_ptr<VertexShader> vertexShader;
    std::unique_ptr<FragmentShader> fragmentShader;
};

template <typename T>
void ShaderProgram::setUniform(const std::string& name, const T& value) {
    auto location = glGetUniformLocation(programIndex, name.c_str());
    if constexpr(std::is_same_v<T, glm::mat4>) {
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
    }
    else if constexpr(std::is_same_v<T, glm::vec4>) {
        glUniform4fv(location, 1, glm::value_ptr(value));
    }
}