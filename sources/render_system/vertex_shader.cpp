#include "render_system/vertex_shader.h"

void VertexShader::init(const std::string& text) {
    if (isValid()) {
        destroy();
    }

    auto cText = text.c_str();
    shaderIndex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderIndex, 1, &cText, NULL);
    glCompileShader(shaderIndex);
}

void VertexShader::destroy() {
    if(isValid()) {
        glDeleteShader(shaderIndex);
        shaderIndex = INVALID_SHADER_INDEX;
    }
}

bool VertexShader::isValid() const {
    return shaderIndex != INVALID_SHADER_INDEX;
}

GLuint VertexShader::getIndex() const {
    return shaderIndex;
}