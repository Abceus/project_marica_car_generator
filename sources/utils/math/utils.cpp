#include "utils/math/utils.h"
#include "utils/math/matrix.h"

Vec3f rotate(const Vec3f& origin, const Rotor3& rot) {
    Vec3f xAxis = {1.0f, 0.0f, 0.0f};
    Vec3f yAxis = {0.0f, 1.0f, 0.0f};
    Vec3f zAxis = {0.0f, 0.0f, 1.0f};

    Matrixf44 transformMatrix(1.0f);

    transformMatrix = Matrixf44::rotate(transformMatrix, zAxis, rot.getRoll());
    xAxis = Matrixf44::apply(transformMatrix, xAxis);
    zAxis = Matrixf44::apply(transformMatrix, zAxis);

    transformMatrix = Matrixf44::rotate(transformMatrix, yAxis, rot.getYaw());
    zAxis = Matrixf44::apply(transformMatrix, zAxis);

    transformMatrix = Matrixf44::rotate(transformMatrix, zAxis, rot.getPitch());

    return Matrixf44::apply(transformMatrix, origin);
}