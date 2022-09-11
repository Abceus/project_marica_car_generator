#include "utils/math/angle.h"
#include <numbers>
#include <cmath>

void Angle::setDegrees(float degrees_) {
    degrees = degrees_;
}

float Angle::getDegrees() const {
    return degrees;
}

void Angle::setRadians(float radians) {
    degrees = (radians * 180.0f) / std::numbers::pi_v<float>;
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

Angle Angle::operator/(int other) const {
    return Angle::fromDegrees(degrees / other);
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

Angle Angle::normalized() const {
    Angle result = *this;
    while(result.degrees < 0) {
        result.degrees += 360.0f;
    }

    result.degrees = std::fmod(degrees, 360.0f);
    return result;
}