#pragma once

#include <QVector>
#include <QSharedPointer>
#include <QScopedPointer>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "updatable.h"

class PhysicWorld : public IUpdatable
{
public:
    PhysicWorld();
    void init();
    QSharedPointer<btRigidBody> addBody( btRigidBody::btRigidBodyConstructionInfo rbInfo );
    void update( float dt ) override;
private:
    QScopedPointer<btDefaultCollisionConfiguration> m_collisionConfiguration;
    QScopedPointer<btCollisionDispatcher> m_dispatcher;
    QScopedPointer<btDbvtBroadphase> m_broadphase;
    QScopedPointer<btSequentialImpulseConstraintSolver> m_solver;
    QScopedPointer<btDiscreteDynamicsWorld> m_dynamicsWorld;
    QVector<QSharedPointer<btRigidBody>> bodies;
};
