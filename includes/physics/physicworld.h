#pragma once

#include <memory>
#include <vector>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

class PhysicWorld
{
public:
    PhysicWorld();
    btRigidBody* addBody( btRigidBody::btRigidBodyConstructionInfo rbInfo );
    void update( float dt );
private:
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btDbvtBroadphase> m_broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
    // TODO: leak
    std::vector<btRigidBody*> bodies;
};
