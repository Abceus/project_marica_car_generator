#pragma once
#include "glm/glm.hpp"
#include "imgui.h"
#include "render_system/camera/camera_controller.h"
#include "widgets/openglview.h"
#include <optional>

class FreeCameraController : public CameraController {
public:
    void update(std::shared_ptr<SceneNode> camera) override;
};