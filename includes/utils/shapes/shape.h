#pragma once

#include "resources/model.h"
#include "btBulletDynamicsCommon.h"
#include <memory>

class IShape {
public:
    virtual Model getModel() const = 0;
    virtual btCollisionShape* createPhysicShape() const = 0;
};