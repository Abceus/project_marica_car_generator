#pragma once
#include "glm/glm.hpp"
#include "render_system/camera/camera_controller.h"
#include "widgets/openglview.h"
#include <optional>


class FreeCameraController : public CameraController {
public:
    FreeCameraController(OpenglView* openglView);
    void onKeyPressed(wxKeyEvent& event) override;
    void onMouseEvent(wxMouseEvent& event) override;

private:
    std::optional<glm::vec2> prevMousePosition;
    OpenglView* openglView = nullptr;
};