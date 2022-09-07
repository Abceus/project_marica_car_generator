#pragma once

#include "utils/math/vec3.h"

class Plane {
public:
    Plane(const Vec3f& normal = Vec3f(0.0f, 0.0f, 1.0f), float distance = 0.0f);

    float getDistanceToPoint(const Vec3f& point) const;

    static Plane createFromPoints(const Vec3f& a, const Vec3f& b, const Vec3f& c);
    static Plane createFromNormalAndPoint(const Vec3f& normal, const Vec3f& point);

private:
    Vec3f normal;
    float distance;
};