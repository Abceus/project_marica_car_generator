#pragma once

#include <memory>

#include "btBulletDynamicsCommon.h"

#include "object.h"

class PhysObject : public Object
{
public:
    PhysObject( std::unique_ptr<Mesh>&& model, float x, float y, float z );
    void update( float dt );
    void setPhysic( btRigidBody* newBody );
    QVector3D getPosition() const override;
    QQuaternion getRotation() const;
private:
    btRigidBody* physic;
};