#include "utils/shapes/box.h"
#include "utils/math/vec3.h"
#include <memory>

Box::Box(const Vec3f& size) : size(size) {}

Model Box::getModel() const {
    Model result;
    auto halfBoxSize = size / 2.0f;
    result.vertices = {{-halfBoxSize.getX(), halfBoxSize.getY(),
                        halfBoxSize.getZ(), 0.0f, 0.0f, 0},
                       {-halfBoxSize.getX(), halfBoxSize.getY(),
                        -halfBoxSize.getZ(), 1.0f, 0.0f, 0},
                       {halfBoxSize.getX(), halfBoxSize.getY(),
                        -halfBoxSize.getZ(), 1.0f, 1.0f, 0},
                       {halfBoxSize.getX(), halfBoxSize.getY(),
                        halfBoxSize.getZ(), 0.0f, 1.0f, 0},

                       {-halfBoxSize.getX(), -halfBoxSize.getY(),
                        halfBoxSize.getZ(), 1.0f, 0.0f, 0},
                       {-halfBoxSize.getX(), -halfBoxSize.getY(),
                        -halfBoxSize.getZ(), 0.0f, 0.0f, 0},
                       {halfBoxSize.getX(), -halfBoxSize.getY(),
                        -halfBoxSize.getZ(), 0.0f, 1.0f, 0},
                       {halfBoxSize.getX(), -halfBoxSize.getY(),
                        halfBoxSize.getZ(), 1.0f, 1.0f, 0}};

    result.faces = {{{0, 1, 2}}, {{0, 2, 3}},

                    {{0, 4, 3}}, {{4, 3, 7}},

                    {{1, 2, 5}}, {{2, 5, 6}},

                    {{4, 5, 6}}, {{4, 7, 6}},

                    {{0, 1, 5}}, {{0, 4, 5}},

                    {{2, 6, 7}}, {{3, 7, 2}}};

    result.materials.emplace_back("");
	return result;
}

#ifdef WITH_PHYSICS
btCollisionShape* Box::createPhysicShape() const {
    auto halfBoxSize = size / 2.0f;
    return new btBoxShape(halfBoxSize.toBtVec3(Vec3Type::Size));
}
#endif