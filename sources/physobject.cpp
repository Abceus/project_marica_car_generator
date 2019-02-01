#include "physobject.h"

PhysObject::PhysObject( std::unique_ptr<Mesh>&& model, float x, float y, float z, float mass, QVector3D size )
    : Object( std::move( model ), x, y, z )
{
//    colShape = std::make_unique<btBoxShape>( btVector3( size.x(), size.y(), size.z() ) );
//    colShape = std::make_unique<btConvexHullShape>();
//
//    auto mod = m_model->getModel();
//    auto verts = mod.vertices;
//    for( auto& vert: verts )
//    {
//        static_cast<btConvexHullShape *>( colShape.get())->addPoint( btVector3( vert.X, vert.Y, vert.Z ) );
//    }
//    physMesh = std::make_unique<btTriangleMesh>();
    btTriangleMesh physMesh;
    auto mod = m_model->getModel();
    auto inds = mod.indices;
    auto verts = mod.vertices;
//    for( auto& i: inds )
//    {
//        physMesh.addTriangle(
//                { verts[i.vertexes[0]].X, verts[i.vertexes[0]].Y, verts[i.vertexes[0]].Z },
//                { verts[i.vertexes[1]].X, verts[i.vertexes[1]].Y, verts[i.vertexes[1]].Z },
//                { verts[i.vertexes[2]].X, verts[i.vertexes[2]].Y, verts[i.vertexes[2]].Z } );
//    }
    new btBvhTriangleMeshShape( &physMesh, true );
    colShape = std::make_unique<btBvhTriangleMeshShape>( &physMesh, true );
    btTransform startTransform;
    startTransform.setIdentity();
    m_mass = mass;
    startTransform.setOrigin( btVector3( x, y, z ) );

    myMotionState = std::make_unique<btDefaultMotionState>( startTransform );
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