#include "physics/physobject.h"

PhysObject::PhysObject( QSharedPointer<Mesh> model, QSharedPointer<SceneNode> node, float mass, QVector3D size )
    : Object( model, node )
{
    colShape = std::make_unique<btBoxShape>( btVector3( size.x(), size.y(), size.z() ) );
    colShape = std::make_unique<btConvexHullShape>();

//    auto verts = model.vertices;
//    for( auto& vert: verts )
//    {
//        static_cast<btConvexHullShape *>( colShape.get())->addPoint( btVector3( vert.X, vert.Y, vert.Z ) );
//    }
    btTransform startTransform;
    startTransform.setIdentity();
    m_mass = mass;
//    startTransform.setOrigin( btVector3( x, y, z ) );

    myMotionState = std::make_unique<btDefaultMotionState>( startTransform );
}

void PhysObject::update( float dt )
{

}

void PhysObject::setPhysic( btRigidBody* newBody )
{
    physic = newBody;
//    physic->getWorldTransform().setOrigin(btVector3(position.x(), position.y(), position.z()));
}

//QVector3D PhysObject::getPosition() const
//{
//    btTransform transform;
//    physic->getMotionState()->getWorldTransform( transform );
//    return QVector3D( transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z() );
//}

QQuaternion PhysObject::getRotation() const
{
    btQuaternion rotation = physic->getWorldTransform().getRotation();
    return { rotation.w(), rotation.x(), rotation.y(), rotation.z() };
}

btRigidBody::btRigidBodyConstructionInfo PhysObject::getConstructionInfo()
{
    bool isDynamic = ( m_mass != 0.f );
    btVector3 localInertia( 0, 0, 0 );
    if ( isDynamic )
    {
        colShape->calculateLocalInertia( m_mass, localInertia );
    }
    return { m_mass, myMotionState.get(), colShape.get(), localInertia };
}
