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

    for( const auto& constraint: constraints )
    {
        m_dynamicsWorld->removeConstraint( constraint.get() );
    }

    bodies.clear();
    constraints.clear();
}

QSharedPointer<btRigidBody> PhysicWorld::addBody( btRigidBody::btRigidBodyConstructionInfo rbInfo )
{
    bodies.append( QSharedPointer<btRigidBody>( new btRigidBody ( rbInfo ) ) );
    m_dynamicsWorld->addRigidBody( bodies.last().get() );
    return bodies.last();
}

void PhysicWorld::addConstraint(btTypedConstraint *constraint, bool disableCollisionsBetweenLinkedBodies)
{
    constraints.append( QSharedPointer<btTypedConstraint>( constraint ) );
    m_dynamicsWorld->addConstraint( constraint, disableCollisionsBetweenLinkedBodies );
}

void PhysicWorld::update( float dt )
{
    m_dynamicsWorld->stepSimulation( dt );
}
