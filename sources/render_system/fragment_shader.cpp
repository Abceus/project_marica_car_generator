#include "render_system/fragment_shader.h"

void FragmentShader::init(const std::string& text) {
    if (isValid()) {
        destroy();
    }

    auto cText = text.c_str();
    shaderIndex = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderIndex, 1, &cText, NULL);
    glCompileShader(shaderIndex);
}

void FragmentShader::destroy() {
    if(isValid()) {
        glDeleteShader(shaderIndex);
        shaderIndex = INVALID_SHADER_INDEX;
    }
}

bool FragmentShader::isValid() const {
    return shaderIndex != INVALID_SHADER_INDEX;
}

GLuint FragmentShader::getIndex() const {
    return shaderIndex;
}