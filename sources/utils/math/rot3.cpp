#include "utils/math/rot3.h"

Rotor3::Rotor3(float pitchDegrees, float yawDegrees, float rollDegrees)
    : pitch(Angle::fromDegrees(pitchDegrees)),
      yaw(Angle::fromDegrees(yawDegrees)),
      roll(Angle::fromDegrees(rollDegrees)) {}

Rotor3::Rotor3(Angle pitch, Angle yaw, Angle roll)
    : pitch(pitch), yaw(yaw), roll(roll) {}

Angle Rotor3::getPitch() const {
    return pitch;
}

Angle Rotor3::getYaw() const {
    return yaw;
}

Angle Rotor3::getRoll() const {
    return roll;
}

void Rotor3::setPitch(Angle pitch_) {
    pitch = pitch_;
}

void Rotor3::setYaw(Angle yaw_) {
    yaw = yaw_;
}

void Rotor3::setRoll(Angle roll_) {
    roll = roll_;
}

glm::vec3 Rotor3::toGLVec3() const {
    return {static_cast<float>(yaw.getDegrees()),
            static_cast<float>(roll.getDegrees()),
            static_cast<float>(-pitch.getDegrees())};
}

Rotor3::Rotor3(const Rotor3& other)
    : pitch(other.pitch), yaw(other.yaw), roll(other.roll) {}

Rotor3& Rotor3::operator=(const Rotor3& other) {
    pitch = other.pitch;
    yaw = other.yaw;
    roll = other.roll;
    return *this;
}

Rotor3 Rotor3::fromGLVec3(const glm::vec3& other) {
    return {-other.z, other.x, other.y};
}

Rotor3 Rotor3::operator+(const Rotor3& other) const {
    return {pitch + other.pitch, yaw + other.yaw, roll + other.roll};
}

Rotor3 Rotor3::operator-(const Rotor3& other) const {
    return {pitch - other.pitch, yaw - other.yaw, roll - other.roll};
}

Rotor3 Rotor3::operator*(const Rotor3& other) const {
    return {pitch * other.pitch, yaw * other.yaw, roll * other.roll};
}

Rotor3 Rotor3::operator/(const Rotor3& other) const {
    return {pitch / other.pitch, yaw / other.yaw, roll / other.roll};
}

Rotor3 Rotor3::operator+(Angle factor) const {
    return {pitch + factor, yaw + factor, roll + factor};
}

Rotor3 Rotor3::operator-(Angle factor) const {
    return {pitch - factor, yaw - factor, roll - factor};
}

Rotor3 Rotor3::operator*(Angle factor) const {
    return {pitch * factor, yaw * factor, roll * factor};
}

Rotor3 Rotor3::operator/(Angle factor) const {
    return {pitch / factor, yaw / factor, roll / factor};
}

bool Rotor3::operator==(const Rotor3& other) const {
    return pitch == other.pitch && yaw == other.yaw && roll == other.roll;
}

// template <typename T>
// Vec3<T> Rotor3<T>::rotate(const Vec3<T>& origin) const {

// }