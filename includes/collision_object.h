#pragma once

#include "resources/resource_pointer.h"
#include "resources/wireframe_model.h"
#include <QSharedPointer>
#ifndef WITHOUT_SIMULATION
#include <btBulletDynamicsCommon.h>
#endif
#include <optional>

struct CollisionObject
{
    WireframeModel model;
#ifndef WITHOUT_SIMULATION
    QSharedPointer<btCollisionShape> shape;
#endif
    static std::optional<CollisionObject> readASE( const QString& path );
};
