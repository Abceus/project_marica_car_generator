#pragma once

#include <memory>

#include "btBulletDynamicsCommon.h"

#include "render_system/scene_node.h"
#include "resources/model.h"
#include "updatable.h"
#include "utils/math/vec3.h"


class PhysObject : public IUpdatable {
public:
    PhysObject(const std::shared_ptr<SceneNode>& node, const Model& physModel,
               float mass, Vec3f size = Vec3f());
    void update(const std::chrono::milliseconds& dt) override;
    void setPhysic(const std::shared_ptr<btRigidBody>& newBody);
    //    QVector3D getPosition() const override;
    // QQuaternion getRotation() const;
    btRigidBody::btRigidBodyConstructionInfo getConstructionInfo();

private:
    std::shared_ptr<btRigidBody> physic;
    std::unique_ptr<btCollisionShape> colShape;
    std::unique_ptr<btDefaultMotionState> myMotionState;
    float m_mass;
    std::shared_ptr<SceneNode> m_node;
    //    std::unique_ptr<btStridingMeshInterface> physMesh;
};
