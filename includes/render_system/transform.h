#pragma once

#include "utils/math/matrix.h"
#include "utils/math/quaternion.h"
#include "utils/math/vec3.h"

class Transform {
public:
    Vec3f getLocation() const;
    void setLocation(const Vec3f& value);

    Quaternion getRotation() const;
    void setRotation(const Quaternion& value);

    Vec3f getScale() const;
    void setScale(const Vec3f& value);
    void setScale(float value);

    Matrixf44 getRotationMatrix() const;
    Matrixf44 getLocationMatrix() const;
    Matrixf44 getScaleMatrix() const;
    Matrixf44 getMatrix() const;

private:
    Vec3f location;
    Quaternion rotation;
    Vec3f scale = {1.0f, 1.0f, 1.0f};
};
