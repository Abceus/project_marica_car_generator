#include "render_system/shader_program.h"
#include <memory>

void ShaderProgram::init(const std::string& vertexShaderText,
                         const std::string& fragmentShaderText) {
    if (isValid()) {
        destroy();
    }

    programIndex = glCreateProgram();

    vertexShader = std::make_unique<VertexShader>();
    vertexShader->init(vertexShaderText);

    fragmentShader = std::make_unique<FragmentShader>();
    fragmentShader->init(fragmentShaderText);

    glAttachShader(programIndex, vertexShader->getIndex());
    glAttachShader(programIndex, fragmentShader->getIndex());

    glLinkProgram(programIndex);

    glDetachShader(programIndex, vertexShader->getIndex());
    glDetachShader(programIndex, fragmentShader->getIndex());
}
void ShaderProgram::destroy() {
    if (vertexShader) {
        vertexShader->destroy();
        vertexShader.reset();
    }
    if (fragmentShader) {
        fragmentShader->destroy();
        fragmentShader.reset();
    }
    glDeleteProgram(programIndex);
}

void ShaderProgram::bind() const {
    glUseProgram(programIndex);
}
void ShaderProgram::unbind() const {
    glUseProgram(INVALID_PROGRAM_INDEX);
}

bool ShaderProgram::isValid() const {
    return programIndex != INVALID_PROGRAM_INDEX;
}