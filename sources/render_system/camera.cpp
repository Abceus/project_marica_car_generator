#include "render_system/camera.h"
#include "utils/math/matrix.h"
#include "utils/math/utils.h"

glm::mat4 Camera::getProjectionMatrix() const {
    Vec3f direction(1.0f, 0.0f, 0.0f);

    // Vec3f zAxis = {0.0f, 0.0f, 1.0f};
    // direction = rotate(direction, getRotation());
    // zAxis = rotate(zAxis, getRotation());

    Matrixf44 m(1.0f);
    // m = m.translate(m, direction);
    auto globalRotation = getGlobalRotation();
    m = m.rotate(m, globalRotation);
    direction = m.apply(m, direction);

    auto loc = getGlobalLocation();

    Vec3f zAxis = {m[0][2], m[1][2], m[2][2]};

    return glm::lookAt(loc.toGLVec3(), (loc + direction).toGLVec3(),
                       globalRotation.getZAxis().toGLVec3());
}
