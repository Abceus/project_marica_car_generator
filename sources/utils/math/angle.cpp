#include "utils/math/angle.h"
#include <numbers>
#include <cmath>

void Angle::setDegrees(float degrees_) {
    degrees = degrees_;
    normalize();
}

float Angle::getDegrees() const {
    return degrees;
}

void Angle::setRadians(float radians) {
    degrees = (radians * 180.0f) / std::numbers::pi_v<float>;
    normalize();
}

float Angle::getRadians() const {
    return (degrees * std::numbers::pi_v<float>) / 180.0f;
}

Angle::Angle(const Angle& other) {
    degrees = other.degrees;
}

Angle& Angle::operator=(const Angle& other) {
    degrees = other.degrees;
    return *this;
}

Angle Angle::operator+(const Angle& other) const {
    return Angle::fromDegrees(degrees + other.degrees);
}
Angle Angle::operator-(const Angle& other) const {
    return Angle::fromDegrees(degrees - other.degrees);
}
Angle Angle::operator*(const Angle& other) const {
    return Angle::fromDegrees(degrees * other.degrees);
}
Angle Angle::operator/(const Angle& other) const {
    return Angle::fromDegrees(degrees / other.degrees);
}

bool Angle::operator==(const Angle& other) const {
    return degrees == other.degrees;
}
bool Angle::operator>(const Angle& other) const {
    return degrees > other.degrees;
}
bool Angle::operator>=(const Angle& other) const {
    return degrees >= other.degrees;
}
bool Angle::operator<(const Angle& other) const {
    return degrees < other.degrees;
}
bool Angle::operator<=(const Angle& other) const {
    return degrees <= other.degrees;
}

Angle Angle::fromDegrees(float degrees) {
    Angle result;
    result.setDegrees(degrees);
    return result;
}
Angle Angle::fromRadians(float radians) {
    Angle result;
    result.setRadians(radians);
    return result;
}

void Angle::normalize() {
    while(degrees < 0) {
        degrees += 360.0f;
    }

    degrees = std::fmod(degrees, 360.0f);
}