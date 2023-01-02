#pragma once
#include <string>
#include <mul_opengl.h>

class VertexShader {
public:
    void init(const std::string& text);
    void destroy();

	bool isValid() const;

	GLuint getIndex() const;
private:
    static constexpr GLuint INVALID_SHADER_INDEX = 0;
    GLuint shaderIndex = INVALID_SHADER_INDEX;
};