#pragma once
#include "imgui.h"
#include "render_system/batch.h"
#include "render_system/camera/camera_controller.h"
#include "render_system/drawable.h"
#include "render_system/mesh.h"
#include "render_system/renderer.h"
#include "render_system/scene.h"
#include "render_system/shader_program.h"
#include "updatable.h"
#include "utils/gl.h"
#include "utils/math/vec3.h"
#include <chrono>
#include <functional>
#include <memory>
#include <optional>
#include <set>

class OpenglView {
public:
    using OpenglInitedCallbackType = std::function<void()>;

public:
    void draw();

    std::weak_ptr<Scene> getScene() const;
    Renderer& getRenderer();

    void addUpdatable(const std::shared_ptr<IUpdatable>& updatable);
    void removeUpdatable(const std::shared_ptr<IUpdatable>& updatable);
    void clearUpdatables();

    void setOpenglInitedCallback(const OpenglInitedCallbackType& callback) {
        openglInitedCallback = callback;
    }

    void setCameraController(std::unique_ptr<CameraController>&& controller) {
        cameraController = std::move(controller);
    }

private:
    Renderer m_renderer;

    void InitGL();
    void ResetProjectionMode();
    void OnResize(const ImVec2& newSize);

    ImVec2 prevAvailSize;

    bool inited = false;

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::set<std::shared_ptr<IUpdatable>> updatables;

    std::chrono::time_point<std::chrono::steady_clock> lastUpdate;

    unsigned int fbo;
    unsigned int texture;
    unsigned int rbo;

    OpenglInitedCallbackType openglInitedCallback;

    std::unique_ptr<CameraController> cameraController;
};