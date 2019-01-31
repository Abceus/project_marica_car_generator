#include "physobject.h"

PhysObject::PhysObject( std::unique_ptr<Mesh>&& model, float x=0.f, float y=0.f, float z=0.f )
    : Object( std::move( model ), x, y, z )
{
}

void PhysObject::update( float dt )
{

}

void PhysObject::setPhysic( btRigidBody* newBody )
{
    physic = newBody;
    physic->getWorldTransform().setOrigin(btVector3(position.x(), position.y(), position.z()));
}

QVector3D PhysObject::getPosition() const
{
    btTransform transform;
    physic->getMotionState()->getWorldTransform( transform );
    return QVector3D( transform.getOrigin().x(), transform.getOrigin().y(), transform.getOrigin().z() );
}

QQuaternion PhysObject::getRotation() const
{
    btQuaternion rotation = physic->getWorldTransform().getRotation();
    return { rotation.w(), rotation.x(), rotation.y(), rotation.z() };
}