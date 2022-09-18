#pragma once

#include "resources/model.h"
#ifdef WITH_PHYSICS
#include "btBulletDynamicsCommon.h"
#endif
#include <memory>

class IShape {
public:
    virtual Model getModel() const = 0;
#ifdef WITH_PHYSICS
    virtual btCollisionShape* createPhysicShape() const = 0;
#endif
};