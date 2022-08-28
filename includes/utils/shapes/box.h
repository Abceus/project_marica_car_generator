#pragma once

#include "utils/math/vec3.h"
#include "utils/shapes/shape.h"


class Box : public IShape {
public:
    Box(const Vec3f& size = {});
    Model getModel() const override;
    btCollisionShape* createPhysicShape() const override;

private:
    Vec3f size;
};