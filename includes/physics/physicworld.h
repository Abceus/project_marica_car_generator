#pragma once

#include <utility>
#include <vector>
#include <memory>

#include "BulletDynamics/ConstraintSolver/btTypedConstraint.h"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

#include "updatable.h"

class PhysicWorld : public IUpdatable
{
public:
    PhysicWorld();
    void init();
    std::shared_ptr<btRigidBody> addBody( btRigidBody::btRigidBodyConstructionInfo rbInfo );

    template<typename T, typename... Args>
    std::shared_ptr<T> addConstraint(Args&&... args);

    void update( const std::chrono::milliseconds& dt ) override;
private:
    std::vector<std::shared_ptr<btRigidBody>> bodies;
    std::vector<std::shared_ptr<btTypedConstraint>> constraints;
    std::unique_ptr<btDefaultCollisionConfiguration> m_collisionConfiguration;
    std::unique_ptr<btCollisionDispatcher> m_dispatcher;
    std::unique_ptr<btDbvtBroadphase> m_broadphase;
    std::unique_ptr<btSequentialImpulseConstraintSolver> m_solver;
    std::unique_ptr<btDiscreteDynamicsWorld> m_dynamicsWorld;
};

template<typename T, typename... Args>
std::shared_ptr<T> PhysicWorld::addConstraint(Args&&... args) {
    constraints.emplace_back(std::make_shared<T>(std::forward<Args>(args)...));
    m_dynamicsWorld->addConstraint(constraints.back().get());
    return std::static_pointer_cast<T>(constraints.back());
}