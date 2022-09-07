#include "utils/math/plane.h"

Plane::Plane(const Vec3f& normal, float distance)
    : normal(normal), distance(distance) {}

float Plane::getDistanceToPoint(const Vec3f& point) const {
    return std::abs(normal.getDotProduct(point) + distance);
}

Plane Plane::createFromPoints(const Vec3f& a, const Vec3f& b, const Vec3f& c) {
    return createFromNormalAndPoint(
        (b - a).getCrossProduct(c - a).getNormalized(), a);
}

Plane Plane::createFromNormalAndPoint(const Vec3f& normal_,
                                      const Vec3f& point) {
    Plane result;
    result.normal = normal_.getNormalized();
    result.distance = -point.getDotProduct(result.normal);
    return result;
}