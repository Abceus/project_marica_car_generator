#pragma once

#include "glm/fwd.hpp"
#include "render_system/scene_node.h"

class Camera : public SceneNode {
public:
	glm::mat4 getProjectionMatrix() const;
};