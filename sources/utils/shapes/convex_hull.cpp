#include "utils/shapes/convex_hull.h"

#include "utils/math/vec3.h"

ConvexHull::ConvexHull(const Model& model) : model(model) {

}

Model ConvexHull::getModel() const {
    return model;
}

btCollisionShape* ConvexHull::createPhysicShape() const {
    auto result = new btConvexHullShape();
    for (auto& vert: model.vertices) {
        result->addPoint(Vec3f(vert.X, vert.Y, vert.Z).toBtVec3());
    }
    return result;
}
