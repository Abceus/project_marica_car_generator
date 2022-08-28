#pragma once

#include "utils/shapes/shape.h"


class ConvexHull : public IShape {
public:
    ConvexHull(const Model& model);
    Model getModel() const override;
    btCollisionShape* createPhysicShape() const override;

private:
    Model model;
};