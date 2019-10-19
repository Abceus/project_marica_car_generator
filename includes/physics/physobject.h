#pragma once

#include <memory>

#include "btBulletDynamicsCommon.h"

#include "object.h"
#include "updatable.h"

class PhysObject : public Object, public IUpdatable
{
public:
    PhysObject( QSharedPointer<Mesh> model, QSharedPointer<SceneNode> node, const Model& physModel, float mass,
            QVector3D size=QVector3D( 0.f, 0.f, 0.f ) );
    void update( float dt ) override;
    void setPhysic( QSharedPointer<btRigidBody> newBody );
//    QVector3D getPosition() const override;
    QQuaternion getRotation() const;
    btRigidBody::btRigidBodyConstructionInfo getConstructionInfo();
private:
    QSharedPointer<btRigidBody> physic;
    std::unique_ptr<btCollisionShape> colShape;
    std::unique_ptr<btDefaultMotionState> myMotionState;
    float m_mass;
//    std::unique_ptr<btStridingMeshInterface> physMesh;
};
