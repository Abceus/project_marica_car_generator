#include "render_system/shader_program.h"
#include <fstream>
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

void ShaderProgram::init(const std::filesystem::path& vertexShaderPath,
                         const std::filesystem::path& fragmentShaderPath) {
    std::ifstream vertexShaderStream(vertexShaderPath);
    std::stringstream vertexShaderBuffer;
    vertexShaderBuffer << vertexShaderStream.rdbuf();

    std::ifstream fragmentShaderStream(fragmentShaderPath);
    std::stringstream fragmentShaderBuffer;
    fragmentShaderBuffer << fragmentShaderStream.rdbuf();

    init(vertexShaderBuffer.str(), fragmentShaderBuffer.str());
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