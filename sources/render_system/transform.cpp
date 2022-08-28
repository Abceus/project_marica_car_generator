#include "render_system/transform.h"
#include "utils/math/matrix.h"

Vec3f Transform::getLocation() const {
    return location;
}

void Transform::setLocation(const Vec3f& value) {
    location = value;
}

Quaternion Transform::getRotation() const {
    return rotation;
}

void Transform::setRotation(const Quaternion& value) {
    rotation = value;
}

Vec3f Transform::getScale() const {
    return scale;
}

void Transform::setScale(const Vec3f& value) {
    scale = value;
}

void Transform::setScale(float value) {
    scale = {value, value, value};
}

Matrixf44 Transform::getRotationMatrix() const {
    return Matrixf44::rotate(Matrixf44(1.0f), rotation);
}

Matrixf44 Transform::getLocationMatrix() const {
    return Matrixf44::translate(Matrixf44(1.0f), location);
}

Matrixf44 Transform::getScaleMatrix() const {
    return Matrixf44::scale(Matrixf44(1.0f), scale);
}

Matrixf44 Transform::getMatrix() const {
    return getLocationMatrix() * getRotationMatrix() * getScaleMatrix();
}
