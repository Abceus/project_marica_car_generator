#include "utils/math/rot3.h"

Rotor3::Rotor3(float rollDegrees, float pitchDegrees, float yawDegrees)
    : roll(Angle::fromDegrees(rollDegrees)),
      pitch(Angle::fromDegrees(pitchDegrees)),
      yaw(Angle::fromDegrees(yawDegrees)) {}

Rotor3::Rotor3(Angle pitch, Angle yaw, Angle roll)
    : roll(roll), pitch(pitch), yaw(yaw) {}

Angle Rotor3::getRoll() const {
    return roll;
}

Angle Rotor3::getPitch() const {
    return pitch;
}

Angle Rotor3::getYaw() const {
    return yaw;
}

void Rotor3::setRoll(Angle roll_) {
    roll = roll_;
}

void Rotor3::setPitch(Angle pitch_) {
    pitch = pitch_;
}

void Rotor3::setYaw(Angle yaw_) {
    yaw = yaw_;
}

glm::vec3 Rotor3::toGLVec3() const {
    return {static_cast<float>(pitch.getDegrees()),
            static_cast<float>(yaw.getDegrees()),
            static_cast<float>(-roll.getDegrees())};
}

Rotor3::Rotor3(const Rotor3& other)
    : roll(other.roll), pitch(other.pitch), yaw(other.yaw) {}

Rotor3& Rotor3::operator=(const Rotor3& other) {
    roll = other.roll;
    pitch = other.pitch;
    yaw = other.yaw;
    return *this;
}

Rotor3 Rotor3::fromGLVec3(const glm::vec3& other) {
    return {-other.z, other.x, other.y};
}

Rotor3 Rotor3::operator+(const Rotor3& other) const {
    return {roll + other.roll, pitch + other.pitch, yaw + other.yaw};
}

Rotor3 Rotor3::operator-(const Rotor3& other) const {
    return {roll - other.roll, pitch - other.pitch, yaw - other.yaw};
}

Rotor3 Rotor3::operator*(const Rotor3& other) const {
    return {roll * other.roll, pitch * other.pitch, yaw * other.yaw};
}

Rotor3 Rotor3::operator/(const Rotor3& other) const {
    return {roll / other.roll, pitch / other.pitch, yaw / other.yaw};
}

Rotor3 Rotor3::operator+(Angle factor) const {
    return {roll + factor, pitch + factor, yaw + factor};
}

Rotor3 Rotor3::operator-(Angle factor) const {
    return {roll - factor, pitch - factor, yaw - factor};
}

Rotor3 Rotor3::operator*(Angle factor) const {
    return {roll * factor, pitch * factor, yaw * factor};
}

Rotor3 Rotor3::operator/(Angle factor) const {
    return {roll / factor, pitch / factor, yaw / factor};
}

bool Rotor3::operator==(const Rotor3& other) const {
    return pitch == other.pitch && yaw == other.yaw && roll == other.roll;
}

// template <typename T>
// Vec3<T> Rotor3<T>::rotate(const Vec3<T>& origin) const {

// }