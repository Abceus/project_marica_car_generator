#include "render_system/camera/free_camera_controller.h"
#include "imgui.h"
#include "utils/math/matrix.h"
#include "utils/math/quaternion.h"
#include "utils/math/utils.h"

void FreeCameraController::update(std::shared_ptr<SceneNode> camera) {
    const auto NoneState = 0b000;
    const auto LeftButtonState = 0b001;
    const auto RightButtonState = 0b010;
    const auto BothButtonState = LeftButtonState | RightButtonState;

    const auto ShiftState = 0b100;
    const auto ShiftLeftButtonState = ShiftState | LeftButtonState;
    const auto ShiftRightButtonState = ShiftState | RightButtonState;
    const auto ShiftBothButtonState = ShiftState | BothButtonState;

    auto controlState = NoneState;
    if (ImGui::GetIO().KeyShift) {
        controlState |= ShiftState;
    }
    if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Left]) {
        controlState |= LeftButtonState;
    }
    if (ImGui::GetIO().MouseDown[ImGuiMouseButton_Right]) {
        controlState |= RightButtonState;
    }

    if (controlState != NoneState) {
        // openglView->SetCursor(wxCursor(wxImage(1, 1)));
        if (ImGui::GetIO().MouseDelta.x != 0 || ImGui::GetIO().MouseDelta.y != 0) {
            auto currentCameraLocation = camera->getLocation();
            auto currentCameraRotation = camera->getRotation().toEulerAngles();
            // openglView->WarpPointer(prevMousePosition->x,
            // prevMousePosition->y);
            auto moveSpeed = 5.0f;
            auto rotateSpeed = 1.0f;

            switch (controlState) {
            case ShiftLeftButtonState: {
                currentCameraLocation.setX(currentCameraLocation.getX() +
                                           ImGui::GetIO().MouseDelta.x * moveSpeed);
                break;
            }
            case ShiftRightButtonState: {
                currentCameraLocation.setY(currentCameraLocation.getY() +
                                           ImGui::GetIO().MouseDelta.x * moveSpeed);
                break;
            }
            case ShiftBothButtonState: {
                currentCameraLocation.setZ(currentCameraLocation.getZ() +
                                           ImGui::GetIO().MouseDelta.x * moveSpeed);
                break;
            }
            case LeftButtonState: {
                currentCameraRotation.setYaw(
                    currentCameraRotation.getYaw() +
                    Angle::fromDegrees(ImGui::GetIO().MouseDelta.x * rotateSpeed));
                currentCameraLocation =
                    currentCameraLocation +
                    Matrixf44::apply(camera->getRotationMatrix(),
                                     Vec3f(-ImGui::GetIO().MouseDelta.y * moveSpeed, 0.0f,
                                           0.0f));
                break;
            }
            case RightButtonState: {
                currentCameraRotation.setYaw(
                    currentCameraRotation.getYaw() +
                    Angle::fromDegrees(ImGui::GetIO().MouseDelta.x * rotateSpeed));
                currentCameraRotation.setPitch(
                    currentCameraRotation.getPitch() +
                    Angle::fromDegrees(ImGui::GetIO().MouseDelta.y * rotateSpeed));
                break;
            }
            case BothButtonState: {
                currentCameraLocation =
                    currentCameraLocation +
                    Matrixf44::apply(camera->getRotationMatrix(),
                                     Vec3f(0.0f, 0.0f, -ImGui::GetIO().MouseDelta.y * moveSpeed));
                break;
            }
            }
            camera->setLocation(currentCameraLocation);
            camera->setRotation(Quaternion::fromEulerAngles(currentCameraRotation));
        }
    }
}
