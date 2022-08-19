#include "render_system/camera.h"
#include "utils/math/utils.h"

glm::mat4 Camera::getProjectionMatrix() const {
    Vec3f direction(1.0f, 0.0f, 0.0f);

    Vec3f zAxis = {0.0f, 0.0f, 1.0f};
    direction = rotate(direction, getRotation());
    zAxis = rotate(zAxis, getRotation());

    return glm::lookAt(getLocation().toGLVec3(),
                       (getLocation() + direction).toGLVec3(),
                       zAxis.toGLVec3());
}
