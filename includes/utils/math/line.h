#pragma once

#include "utils/math/vec3.h"

class Line {
public:
    Line(const Vec3f& start = Vec3f(), const Vec3f& end = Vec3f());

    float length() const;
    float getDistanceToPoint(const Vec3f& point) const;

private:
    Vec3f start;
    Vec3f end;
};