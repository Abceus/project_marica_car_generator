#include "utils/shapes/convex_hull.h"

#include "LinearMath/btVector3.h"
#include "resources/wireframe_model.h"
#include "utils/math/vec3.h"
#include <memory>

#include "utils/math/geometry_prototype.h"

ConvexHull::ConvexHull(const std::vector<Vertex>& model) : model(model) {}

Model ConvexHull::getModel() const {
    std::vector<Vec3f> v;
    for(const auto& vert: model) {
        v.push_back({vert.X, vert.Y, vert.Z});
    }
    return getConvexHull(v);
}

btCollisionShape* ConvexHull::createPhysicShape() const {
    auto result = new btConvexHullShape();
    for (auto& vert : model) {
        result->addPoint(Vec3f(vert.X, vert.Y, vert.Z).toBtVec3());
    }
    return result;
}
