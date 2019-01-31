#include "physicworld.h"

PhysicWorld::PhysicWorld()
{
    m_collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
    m_dispatcher = std::make_unique<btCollisionDispatcher>( m_collisionConfiguration.get() );
    m_broadphase = std::make_unique<btDbvtBroadphase>();
    m_solver = std::make_unique<btSequentialImpulseConstraintSolver>();
    m_dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>( m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get() );
    m_dynamicsWorld->setGravity( btVector3( 0, -10, 0 ) );
}


btRigidBody* PhysicWorld::addBody( btRigidBody::btRigidBodyConstructionInfo rbInfo )
{
    bodies.emplace_back( new btRigidBody ( rbInfo ) );
    m_dynamicsWorld->addRigidBody( bodies.back() );
    return bodies.back();
}

void PhysicWorld::update( float dt )
{
    m_dynamicsWorld->stepSimulation( dt );
}