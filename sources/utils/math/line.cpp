#include "utils/math/line.h"

Line::Line(const Vec3f& start, const Vec3f& end) : start(start), end(end) {}

float Line::length() const {
    return (end - start).length();
}

float Line::getDistanceToPoint(const Vec3f& point) const {
    auto a = (end - start).length();
    auto b = (point - end).length();
    auto c = (start - point).length();
    auto p = (a + b + c) / 2.0f;
    auto s = std::sqrt(p * (p - a) * (p - b) * (p - c));
    return (2.0f * s) / a;
}