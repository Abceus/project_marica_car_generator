#pragma once

#include "glm/fwd.hpp"
#include <LinearMath/btQuaternion.h>

#include "utils/math/vec3.h"
#include "utils/math/rot3.h"

class Quaternion {
public:
    Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f);

    float getX() const;
    void setX(float value);

    float getY() const;
    void setY(float value);

    float getZ() const;
    void setZ(float value);

    float getW() const;
    void setW(float value);

    Vec3f getXAxis() const;
    Vec3f getYAxis() const;
    Vec3f getZAxis() const;

    glm::quat toGLQuat() const;
    static Quaternion fromGLQuat(const glm::quat& other);

    btQuaternion toBtQuat() const;
    static Quaternion fromBtQuat(const btQuaternion& other);

    Rotor3 toEulerAngles() const;
    static Quaternion fromEulerAngles(const Rotor3& angles);

private:
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 1.0f;
};