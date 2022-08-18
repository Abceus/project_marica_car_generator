#include "render_system/camera_manager.h"

void CameraManager::addCamera(const std::string& alias,
                              const std::shared_ptr<Camera>& newCamera) {
    cameras.emplace(alias, newCamera);
}

void CameraManager::setActiveCamera(const std::string& alias) {
    auto foundCamera = cameras.find(alias);
    if (foundCamera != std::end(cameras)) {
        activeCamera = foundCamera->second;
    } else {
        activeCamera.reset();
    }
}

std::shared_ptr<Camera> CameraManager::getActiveCamera() const {
    return activeCamera;
}
