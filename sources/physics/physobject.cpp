#include "physics/physobject.h"
#include <QtMath>

PhysObject::PhysObject( QSharedPointer<Drawable> drawable, QSharedPointer<SceneNode> node, const Model& physModel, float mass )
    : Object( drawable, node )
{
    colShape = std::make_unique<btConvexHullShape>();

    auto verts = physModel.vertices;
//    for( auto& vert: verts )
//    {
//        static_cast<btConvexHullShape *>( colShape.get())->addPoint( btVector3( vert.X, vert.Y, vert.Z ) );
//    }
    auto indices = physModel.triangles;
    for( const auto& indice: indices )
    {
        for( const auto& index: indice.vertexes )
        {
            const auto& vert = verts[index];
            static_cast<btConvexHullShape *>( colShape.get() )->addPoint( btVector3( vert.X, vert.Y, vert.Z ) );
        }
    }
    btTransform startTransform;
    startTransform.setIdentity();
    m_mass = mass;
    auto location = node->getLocation();
//    startTransform.setRotation( btQuaternion( qDegreesToRadians( node->getParentRotation().y() ), qDegreesToRadians( node->getParentRotation().x() ), qDegreesToRadians( node->getParentRotation().z() ) ) );
    startTransform.setOrigin( location.getBulletVector() );
    startTransform.setRotation( btQuaternion( qDegreesToRadians( node->getOriginRotation().y() ), qDegreesToRadians( node->getOriginRotation().x() ), qDegreesToRadians( node->getOriginRotation().z() ) ) );
//    startTransform.setRotation( btQuaternion( qDegreesToRadians( node->getOriginRotation().z() ), qDegreesToRadians( node->getOriginRotation().y() ), qDegreesToRadians( -node->getOriginRotation().x() ) ) );

    myMotionState = std::make_unique<btDefaultMotionState>( startTransform );
}

void PhysObject::update( float dt )
{
    btTransform transform;
    physic->getMotionState()->getWorldTransform( transform );
    m_node->setLocation( transform.getOrigin() );

    btQuaternion rotation = physic->getWorldTransform().getRotation();
    m_node->setRotation( QQuaternion( rotation.w(), rotation.x(), rotation.y(), rotation.z() ).toEulerAngles() );
}

void PhysObject::setPhysic( QSharedPointer<btRigidBody> newBody )
{
    physic = newBody;
    //    physic->getWorldTransform().setOrigin(btVector3(position.x(), position.y(), position.z()));
}

QSharedPointer<btRigidBody> PhysObject::getPhysic() const
{
    return physic;
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
