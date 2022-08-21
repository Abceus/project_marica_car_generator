#include "render_system/camera/free_camera_controller.h"
#include "utils/math/utils.h"

FreeCameraController::FreeCameraController(OpenglView* openglView)
    : openglView(openglView) {
    openglView->Bind(wxEVT_MOTION, &FreeCameraController::onMouseEvent, this);
}

void FreeCameraController::onKeyPressed(wxKeyEvent& event) {}

void FreeCameraController::onMouseEvent(wxMouseEvent& event) {
    const auto NoneState = 0b000;
    const auto LeftButtonState = 0b001;
    const auto RightButtonState = 0b010;
    const auto BothButtonState = LeftButtonState | RightButtonState;

    const auto ShiftState = 0b100;
    const auto ShiftLeftButtonState = ShiftState | LeftButtonState;
    const auto ShiftRightButtonState = ShiftState | RightButtonState;
    const auto ShiftBothButtonState = ShiftState | BothButtonState;

    auto controlState = NoneState;
    if (wxGetKeyState(WXK_SHIFT)) {
        controlState |= ShiftState;
    }
    if (event.LeftIsDown()) {
        controlState |= LeftButtonState;
    }
    if (event.RightIsDown()) {
        controlState |= RightButtonState;
    }
    if (controlState != NoneState) {
        openglView->SetCursor(wxCursor(wxImage(1, 1)));
        if (event.Dragging()) {
            auto currentCamera =
                openglView->getScene().lock()->getActiveCamera();
            auto currentCameraLocation = currentCamera->getLocation();
            auto currentCameraRotation = currentCamera->getRotation();
            auto delta = glm::vec2();
            auto currentMousePosition = glm::vec2{event.GetX(), event.GetY()};
            if (prevMousePosition) {
                delta = currentMousePosition - prevMousePosition.value();
            } else {
                prevMousePosition = currentMousePosition;
            }
            openglView->WarpPointer(prevMousePosition->x, prevMousePosition->y);
            auto moveSpeed = 5.0f;
            auto rotateSpeed = 1.0f;

            switch (controlState) {
            case ShiftLeftButtonState: {
                currentCameraLocation.setX(currentCameraLocation.getX() +
                                           delta.x * moveSpeed);
                break;
            }
            case ShiftRightButtonState: {
                currentCameraLocation.setY(currentCameraLocation.getY() +
                                           delta.x * moveSpeed);
                break;
            }
            case ShiftBothButtonState: {
                currentCameraLocation.setZ(currentCameraLocation.getZ() +
                                           delta.x * moveSpeed);
                break;
            }
            case LeftButtonState: {
                currentCameraRotation.setRoll(
                    currentCameraRotation.getRoll() +
                    Angle::fromDegrees(delta.x * rotateSpeed));
                currentCameraLocation =
                    currentCameraLocation +
                    rotate(Vec3f(-delta.y * moveSpeed, 0.0f, 0.0f),
                           currentCameraRotation);
                break;
            }
            case RightButtonState: {
                currentCameraRotation.setRoll(
                    currentCameraRotation.getRoll() +
                    Angle::fromDegrees(delta.x * rotateSpeed));
                currentCameraRotation.setYaw(
                    currentCameraRotation.getYaw() +
                    Angle::fromDegrees(delta.y * rotateSpeed));
                break;
            }
            case BothButtonState: {
                currentCameraLocation = currentCameraLocation +
                                        rotate(Vec3f(0.0f, delta.x * moveSpeed,
                                                     -delta.y * moveSpeed),
                                               currentCameraRotation);
                break;
            }
            }
            currentCamera->setLocation(currentCameraLocation);
            currentCamera->setRotation(currentCameraRotation);
        }
    } else {
        prevMousePosition = std::nullopt;
        openglView->SetCursor(*wxCROSS_CURSOR);
    }
}
