#pragma once
#include "imgui.h"
#include "render_system/batch.h"
#include "render_system/camera/camera_controller.h"
#include "render_system/drawable.h"
#include "render_system/mesh.h"
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
#include <vector>

class OpenglView {
public:
    using OpenglInitedCallbackType = std::function<void()>;
    using KeyCallbackType = std::function<void(ImGuiKey)>;

public:
    void draw();

    std::weak_ptr<Scene> getScene() const;

    void addUpdatable(const std::shared_ptr<IUpdatable>& updatable);
    void removeUpdatable(const std::shared_ptr<IUpdatable>& updatable);
    void clearUpdatables();

    void setOpenglInitedCallback(const OpenglInitedCallbackType& callback) {
        openglInitedCallback = callback;
    }

    void setCameraController(std::unique_ptr<CameraController>&& controller) {
        cameraController = std::move(controller);
    }

    void setPressKeyCallback(const KeyCallbackType& callback) {
        pressKeyCallback = callback;
    }

    void setReleaseKeyCallback(const KeyCallbackType& callback) {
        releaseKeyCallback = callback;
    }

    void addKeyForEventsEmit(ImGuiKey key) {
        keysForEventsEmit.emplace_back(key);
    }

private:
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

    std::vector<ImGuiKey> keysForEventsEmit;
    KeyCallbackType pressKeyCallback;
    KeyCallbackType releaseKeyCallback;
};