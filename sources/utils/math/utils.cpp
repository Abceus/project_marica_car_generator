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

Quaternion getQuaternionFromMatrix(const Matrixf44& matrix) {
    Quaternion q;
    auto trace = matrix[0][0] + matrix[1][1] +
                  matrix[2][2];
    if (trace > 0) {
        auto s = 0.5f / sqrtf(trace + 1.0f);
        q.setW(0.25f / s);
        q.setX((matrix[2][1] - matrix[1][2]) * s);
        q.setY((matrix[0][2] - matrix[2][0]) * s);
        q.setZ((matrix[1][0] - matrix[0][1]) * s);
    } else {
        if (matrix[0][0] > matrix[1][1] && matrix[0][0] > matrix[2][2]) {
            auto s = 2.0f * sqrtf(1.0f + matrix[0][0] - matrix[1][1] - matrix[2][2]);
            q.setW((matrix[2][1] - matrix[1][2]) / s);
            q.setX(0.25f * s);
            q.setY((matrix[0][1] + matrix[1][0]) / s);
            q.setZ((matrix[0][2] + matrix[2][0]) / s);
        } else if (matrix[1][1] > matrix[2][2]) {
            float s = 2.0f * sqrtf(1.0f + matrix[1][1] - matrix[0][0] - matrix[2][2]);
            q.setW((matrix[0][2] - matrix[2][0]) / s);
            q.setX((matrix[0][1] + matrix[1][0]) / s);
            q.setY(0.25f * s);
            q.setZ((matrix[1][2] + matrix[2][1]) / s);
        } else {
            float s = 2.0f * sqrtf(1.0f + matrix[2][2] - matrix[0][0] - matrix[1][1]);
            q.setW((matrix[1][0] - matrix[0][1]) / s);
            q.setX((matrix[0][2] + matrix[2][0]) / s);
            q.setY((matrix[1][2] + matrix[2][1]) / s);
            q.setZ(0.25f * s);
        }
    }
    return q;
}