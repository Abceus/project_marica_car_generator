#pragma once

#include "utils/math/vec3.h"
#include "utils/shapes/shape.h"


class Sphere : public IShape {
public:
    Sphere(float radius);
    Model getModel() const override;
    btCollisionShape* createPhysicShape() const override;

private:
    float radius;
};