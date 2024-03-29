#pragma once

#include "resources/wireframe_model.h"
#include "utils/shapes/shape.h"


class ConvexHull : public IShape {
public:
    ConvexHull(const std::vector<Vertex>& model);
    Model getModel() const override;
#ifdef WITH_PHYSICS
    btCollisionShape* createPhysicShape() const override;
#endif
private:
    std::vector<Vertex> model;
};