#pragma once

#include <memory>

#include "btBulletDynamicsCommon.h"

#include "object.h"

class PhysObject : public Object
{
public:
    PhysObject( QSharedPointer<Mesh> model, QSharedPointer<SceneNode> node, float mass,
            QVector3D size=QVector3D( 0.f, 0.f, 0.f ) );
    void update( float dt );
    void setPhysic( btRigidBody* newBody );
//    QVector3D getPosition() const override;
    QQuaternion getRotation() const;
    btRigidBody::btRigidBodyConstructionInfo getConstructionInfo();
private:
    btRigidBody* physic;
    std::unique_ptr<btCollisionShape> colShape;
    std::unique_ptr<btDefaultMotionState> myMotionState;
    float m_mass;
//    std::unique_ptr<btStridingMeshInterface> physMesh;
};
