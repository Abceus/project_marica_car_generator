#pragma once

#include "drawable.h"
#include "garbage_collector.h"
#include "render_system/fragment_shader.h"
#include "render_system/shader_program.h"
#include "render_system/vertex_shader.h"
#include "scene.h"
#include "utils/scope_guard.h"
#include <map>
#include <stack>

class Renderer {
public:
    // Renderer(wxGLContext* context = nullptr, wxGLCanvas* surface = nullptr);
    Renderer() = default;
    void draw(const std::shared_ptr<Scene>& scene);

    template <typename T, typename... Args>
    std::shared_ptr<T> makeDrawable(Args&&... args);

    // std::shared_ptr<VertexShader> loadVertexShader(const std::string& path);
    // std::shared_ptr<FragmentShader> loadFragmentShader(const std::string&
    // path); std::shared_ptr<ShaderProgram> getShaderProgram(const
    // std::shared_ptr<VertexShader>& vertexShader,
    //                  const std::shared_ptr<FragmentShader>& fragmentShader);
    std::shared_ptr<ShaderProgram>
    getShaderProgram(const std::string& vertexShaderPath,
                     const std::string& fragmentShaderPath);

    ScopeGuard pushContextScoped();

    static void pushRenderer(Renderer* newRenderer);
    static void popRenderer();
    static Renderer* getCurrentRenderer();

    // void setContext(wxGLContext* context);
    // void setSurface(wxGLCanvas* surface);
private:
    void makeCurrent();
    void done();
    // wxGLContext* m_context;
    // wxGLCanvas* m_surface;
    GarbageCollector gc;

    // TODO: rework
    // std::map<std::string, std::shared_ptr<VertexShader>> m_vertexShaders;
    // std::map<std::string, std::shared_ptr<FragmentShader>> m_fragmentShaders;
    // std::map<std::pair<std::shared_ptr<VertexShader>,
    //                    std::shared_ptr<FragmentShader>>,
    //          std::shared_ptr<ShaderProgram>>
    //     m_programs;

    static std::stack<Renderer*> rendererStack;
};

template <typename T, typename... Args>
std::shared_ptr<T> Renderer::makeDrawable(Args&&... args) {
    auto contextGuard = pushContextScoped();
    std::shared_ptr<T> result(new T(std::forward<Args>(args)...),
                              [this](T* obj) { gc.addResource(obj); });
    return result;
}