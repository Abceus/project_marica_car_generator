#pragma once

#include <vector>
#include <memory>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "updatable.h"

class PhysicWorld : public IUpdatable
{
public:
    PhysicWorld();
    void init();
    std::shared_ptr<btRigidBody> addBody( btRigidBody::btRigidBodyConstructionInfo rbInfo );
    void update( float dt ) override;
private:
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btDbvtBroadphase> m_broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
    std::vector<std::shared_ptr<btRigidBody>> bodies;
};
