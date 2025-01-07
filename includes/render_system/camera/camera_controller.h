#pragma once

#include "render_system/scene.h"

class CameraController {
public:
    virtual void update(std::shared_ptr<SceneNode> camera) = 0;
};