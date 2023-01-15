#pragma once
#include "mul_callback_owner.h"
#include "render_system/batch.h"
#include "render_system/mesh.h"
#include "render_system/renderer.h"
#include "render_system/scene.h"
#include "render_system/shader_program.h"
#include "updatable.h"
#include "widgets/mul_opengl_canvas.h"
#include <mul_opengl.h>
#include <chrono>
#include <memory>
#include <optional>
#include <set>

class OpenglView : public MulOpenglCanvas {
public:
    OpenglView();

    //void OnSize(wxSizeEvent& event);
    //void OnPaint(wxPaintEvent& event);

    //void onKeyDown(wxKeyEvent& event);
    //void onKeyUp(wxKeyEvent& event);

    //void onTimer(wxTimerEvent& event);

    void glDraw() override;
    void onInit() override;
    void onKeyPressed(const ImGuiInputEvent& event) override;

    std::weak_ptr<Scene> getScene() const;
    Renderer& getRenderer();

    void addUpdatable(const std::shared_ptr<IUpdatable>& updatable);
    void removeUpdatable(const std::shared_ptr<IUpdatable>& updatable);
    void clearUpdatables();

    using OpenglInitedCallbackType = std::shared_ptr<MulCallbackOwner<>>;
    void addOpenglInitedCallback(OpenglInitedCallbackType& outHandler, const std::function<void()>& callback);
private:
    Renderer m_renderer;

    void InitGL();
    void ResetProjectionMode();

    bool inited = false;

    //wxGLContext* m_glRC = nullptr;

    // std::shared_ptr<ShaderProgram> shaderProgram;
    // std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Scene> scene;

    //wxTimer redrawTimer;
    //wxTimer updateTimer;

    // glm::mat4 projectionMatrix;

    // std::shared_ptr<SceneNode> newNode;

    // std::optional<glm::vec2> prevMousePosition;

    std::set<std::shared_ptr<IUpdatable>> updatables;

    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;

    MulCallbackCollection<> openglInitedCollection;
};