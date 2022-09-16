#include "utils/math/quaternion.h"
#include "glm/detail/type_quat.hpp"
#include "utils/math/matrix.h"

#include <numbers>

Quaternion::Quaternion(float x, float y, float z, float w)
    : x(x), y(y), z(z), w(w) {}

float Quaternion::getX() const {
    return x;
}
void Quaternion::setX(float value) {
    x = value;
}

float Quaternion::getY() const {
    return y;
}
void Quaternion::setY(float value) {
    y = value;
}

float Quaternion::getZ() const {
    return z;
}
void Quaternion::setZ(float value) {
    z = value;
}

float Quaternion::getW() const {
    return w;
}
void Quaternion::setW(float value) {
    w = value;
}

Vec3f Quaternion::getXAxis() const {
    Matrixf44 m(1.0f);
    m = m.rotate(m, *this);
    return {m[0][0], m[1][0], m[2][0]};
}

Vec3f Quaternion::getYAxis() const {
    Matrixf44 m(1.0f);
    m = m.rotate(m, *this);
    return {m[0][1], m[1][1], m[2][1]};
}

Vec3f Quaternion::getZAxis() const {
    Matrixf44 m(1.0f);
    m = m.rotate(m, *this);
    return {m[0][2], m[1][2], m[2][2]};
}

glm::quat Quaternion::toGLQuat() const {
    return {w, y, z, -x};
}
Quaternion Quaternion::fromGLQuat(const glm::quat& other) {
    return {-other.z, other.x, other.y, other.w};
}

btQuaternion Quaternion::toBtQuat() const {
    return {y, z, -x, -w};
}

Quaternion Quaternion::fromBtQuat(const btQuaternion& other) {
    return {-other.z(), other.x(), other.y(), -other.w()};
}

Rotor3 Quaternion::toEulerAngles() const {
    Rotor3 result;

    auto sinrCosp = 2.0f * (w * x + y * z);
    auto cosrCosp = 1.0f - 2.0f * (x * x + y * y);
    result.setRoll(Angle::fromRadians(std::atan2(sinrCosp, cosrCosp)));

    auto sinp = 2.0f * (w * y - z * x);
    if (std::abs(sinp) >= 1.0f) {
        result.setPitch(Angle::fromRadians(
            std::copysign(std::numbers::pi_v<float> / 2.0f, sinp)));
    } else {
        result.setPitch(Angle::fromRadians(std::asin(sinp)));
    }

    auto sinyCosp = 2.0f * (w * z + x * y);
    auto cosyCosp = 1.0f - 2.0f * (y * y + z * z);
    result.setYaw(Angle::fromRadians(std::atan2(sinyCosp, cosyCosp)));

    return result;
}

Quaternion Quaternion::fromEulerAngles(const Rotor3& angles) {
    auto cy = cos(angles.getYaw().getRadians() * 0.5f);
    auto sy = sin(angles.getYaw().getRadians() * 0.5f);
    auto cp = cos(angles.getPitch().getRadians() * 0.5f);
    auto sp = sin(angles.getPitch().getRadians() * 0.5f);
    auto cr = cos(angles.getRoll().getRadians() * 0.5f);
    auto sr = sin(angles.getRoll().getRadians() * 0.5f);

    Quaternion q;
    q.w = cr * cp * cy + sr * sp * sy;
    q.x = sr * cp * cy - cr * sp * sy;
    q.y = cr * sp * cy + sr * cp * sy;
    q.z = cr * cp * sy - sr * sp * cy;

    return q;
}