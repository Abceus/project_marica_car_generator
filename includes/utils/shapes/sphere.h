#pragma once

#include "utils/math/vec3.h"
#include "utils/shapes/shape.h"


class Sphere : public IShape {
public:
    Sphere(float radius);
    Model getModel() const override;
#ifdef WITH_PHYSICS
    btCollisionShape* createPhysicShape() const override;
#endif

private:
    float radius;
};