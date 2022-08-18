#pragma once

#include <map>
#include <memory>
#include <string>


#include "render_system/camera.h"

class CameraManager {
public:
    void addCamera(const std::string& alias,
                   const std::shared_ptr<Camera>& newCamera);

    void setActiveCamera(const std::string& alias);
    std::shared_ptr<Camera> getActiveCamera() const;
private:
    std::map<std::string, std::shared_ptr<Camera>> cameras;
    std::shared_ptr<Camera> activeCamera;
};