#include "physics/physicworld.h"

PhysicWorld::PhysicWorld()
    : m_collisionConfiguration( new btDefaultCollisionConfiguration )
    , m_dispatcher( new btCollisionDispatcher( m_collisionConfiguration.get() ) )
    , m_broadphase( new btDbvtBroadphase )
    , m_solver( new btSequentialImpulseConstraintSolver )
    , m_dynamicsWorld( new btDiscreteDynamicsWorld( m_dispatcher.get(), m_broadphase.get(), m_solver.get(), m_collisionConfiguration.get() ) )
{
    m_dynamicsWorld->setGravity( btVector3( 0, -10, 0 ) );
}

void PhysicWorld::init()
{
    for( const auto& body: bodies )
    {
        m_dynamicsWorld->removeRigidBody( body.get() );
    }

    bodies.clear();
}

std::shared_ptr<btRigidBody> PhysicWorld::addBody( btRigidBody::btRigidBodyConstructionInfo rbInfo )
{
    bodies.emplace_back( std::make_shared<btRigidBody>( rbInfo ) );
    m_dynamicsWorld->addRigidBody( bodies.back().get() );
    return bodies.back();
}

void PhysicWorld::update( const std::chrono::milliseconds& dt )
{
    m_dynamicsWorld->stepSimulation( dt.count() / 1000.0f );
}
