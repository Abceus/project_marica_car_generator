#include "render_system/renderer.h"
#include "utils/scope_guard.h"
#include <fstream>
#include <sstream>

std::stack<Renderer*> Renderer::rendererStack;

Renderer::Renderer(wxGLContext* context, wxGLCanvas* surface)
    : m_context(context), m_surface(surface) {}

void Renderer::draw(const std::shared_ptr<Scene>& scene) {
    auto contextGuard = pushContextScoped();
    gc.collect();
    scene->draw();
}

// std::shared_ptr<VertexShader>
// Renderer::loadVertexShader(const std::string& path) {
//     auto found = m_vertexShaders.find(path);

//     if (found == m_vertexShaders.end()) {
//         makeCurrent();
//         std::shared_ptr<VertexShader> shader(new VertexShader());

//         std::ifstream shaderStream(path);
//         std::stringstream shaderBuffer;
//         shaderBuffer << shaderStream.rdbuf();

//         // bool success =
//         shader->init(shaderBuffer.str());
//         // if (!success) {
//         //     qDebug() << "The vertex shader wasn't compiled";
//         //     return nullptr;
//         // }
//         m_vertexShaders.emplace(path, shader);
//         return shader;
//     }

//     return found->second;
// }

// std::shared_ptr<FragmentShader>
// Renderer::loadFragmentShader(const std::string& path) {
//     auto found = m_fragmentShaders.find(path);

//     if (found == m_fragmentShaders.end()) {
//         makeCurrent();
//         std::shared_ptr<FragmentShader> shader(new FragmentShader());

//         std::ifstream shaderStream(path);
//         std::stringstream shaderBuffer;
//         shaderBuffer << shaderStream.rdbuf();

//         // bool success =
//         shader->init(shaderBuffer.str());

//         // if (!success) {
//         //     qDebug() << "The vertex shader wasn't compiled";
//         //     return nullptr;
//         // }
//         m_fragmentShaders.emplace(path, shader);
//         return shader;
//     }

//     return found->second;
// }

// std::shared_ptr<ShaderProgram> Renderer::getShaderProgram(
//     const std::shared_ptr<VertexShader>& vertexShader,
//     const std::shared_ptr<FragmentShader>& fragmentShader) {
//     makeCurrent();
//     auto found = m_programs.find(std::pair<std::shared_ptr<VertexShader>,
//                                            std::shared_ptr<FragmentShader>>(
//         vertexShader, fragmentShader));
//     if (found == m_programs.end()) {
//         std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram);
//         shaderProgram->init(vertexShader, fragmentShader);
//         // if (!shaderProgram->link()) {
//         //     qDebug() << "The shader program wasn't linked";
//         //     return nullptr;
//         // }
//         m_programs.emplace(std::pair<std::shared_ptr<VertexShader>,
//                                      std::shared_ptr<FragmentShader>>(
//                                vertexShader, fragmentShader),
//                            shaderProgram);
//         return shaderProgram;
//     }
//     return *found;
// }

std::shared_ptr<ShaderProgram>
Renderer::getShaderProgram(const std::string& vertexShaderPath,
                           const std::string& fragmentShaderPath) {
    std::shared_ptr<ShaderProgram> shaderProgram(new ShaderProgram);

    std::ifstream vertexShaderStream(vertexShaderPath);
    std::stringstream vertexShaderBuffer;
    vertexShaderBuffer << vertexShaderStream.rdbuf();

    std::ifstream fragmentShaderStream(fragmentShaderPath);
    std::stringstream fragmentShaderBuffer;
    fragmentShaderBuffer << fragmentShaderStream.rdbuf();

    shaderProgram->init(vertexShaderBuffer.str(), fragmentShaderBuffer.str());
    return shaderProgram;
}

void Renderer::makeCurrent() {
    m_context->SetCurrent(*m_surface);
}

void Renderer::done() {
    // m_context->doneCurrent();
}

ScopeGuard Renderer::pushContextScoped() {
    pushRenderer(this);
    return ScopeGuard([this]() { popRenderer(); });
}

void Renderer::pushRenderer(Renderer* newRenderer) {
    rendererStack.push(newRenderer);
    newRenderer->makeCurrent();
}

void Renderer::popRenderer() {
    rendererStack.top()->done();
    rendererStack.pop();
    if (!rendererStack.empty()) {
        rendererStack.top()->makeCurrent();
    }
}

Renderer* Renderer::getCurrentRenderer() {
    return rendererStack.top();
}

void Renderer::setContext(wxGLContext* context) {
    m_context = context;
}

void Renderer::setSurface(wxGLCanvas* surface) {
    m_surface = surface;
}