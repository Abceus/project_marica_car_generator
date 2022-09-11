#pragma once

#include <memory>

#include "btBulletDynamicsCommon.h"

#include "render_system/scene_node.h"
#include "resources/model.h"
#include "updatable.h"
#include "utils/math/vec3.h"
#include "utils/shapes/shape.h"


class PhysObject : public IUpdatable {
public:
    PhysObject(const std::shared_ptr<SceneNode>& node, const std::shared_ptr<IShape>& shape);
    void update(const std::chrono::milliseconds& dt) override;
    void setPhysic(const std::shared_ptr<btRigidBody>& newBody);
    std::shared_ptr<btRigidBody> getPhysics() const;
    btRigidBody::btRigidBodyConstructionInfo getConstructionInfo();

    void setMass(float value);
    float getMass() const;

    void setFriction(float value);
    float getFriction() const;
private:
    std::shared_ptr<btRigidBody> physic;
    std::unique_ptr<btCollisionShape> colShape;
    std::unique_ptr<btDefaultMotionState> myMotionState;
    float m_mass = 0.0f;
    float friction = 0.0f;
    std::shared_ptr<SceneNode> m_node;
    std::shared_ptr<IShape> shape;
};
