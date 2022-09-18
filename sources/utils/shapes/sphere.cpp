#include "utils/shapes/sphere.h"
#include "utils/math/angle.h"
#include <memory>

Sphere::Sphere(float radius) : radius(radius) {}

Model Sphere::getModel() const {
    Model result;
    size_t segments = 16;
    Angle deltaSegment = Angle::fromDegrees(360 / segments);
    Angle deltaRing = Angle::fromDegrees(360 / (segments / 2.0f));
    for (auto ring_num = 0; ring_num < segments / 2; ++ring_num) {
        for (auto segment_num = 0; segment_num < segments; ++segment_num) {
            result.vertices.push_back(
                {std::cos(segment_num * deltaSegment.getRadians()) *
                     std::cos(ring_num * deltaRing.getRadians()) * radius,
                 std::sin(ring_num * deltaRing.getRadians()) *
                     std::cos(segment_num * deltaSegment.getRadians()) * radius,
                 std::sin(segment_num * deltaSegment.getRadians()) * radius,
                 (ring_num % 4) * 0.25f, (segment_num % 4) * 0.25f, 0});
        }
    }

    for (auto ring_num = 0; ring_num < segments / 2 - 1; ++ring_num) {
        for (auto segment_num = 0; segment_num < segments-1; ++segment_num) {
            GLuint currentIndex = ring_num * segments + segment_num;
            result.faces.push_back(
                {{currentIndex, currentIndex + 1,
                  currentIndex + static_cast<GLuint>(segments)}});
            result.faces.push_back(
                {{currentIndex + 1,
                  currentIndex + static_cast<GLuint>(segments),
                  currentIndex + static_cast<GLuint>(segments) + 1}});
        }
    }

    result.materials.emplace_back("");
    return result;
}

#ifdef WITH_PHYSICS
btCollisionShape* Sphere::createPhysicShape() const {
    return new btSphereShape(radius);
}
#endif