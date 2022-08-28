#include "physics/physobject.h"
#include "utils/math/quaternion.h"
#include "utils/math/rot3.h"
#include "utils/math/vec3.h"
#include "wx/log.h"
#include <memory>

PhysObject::PhysObject(const std::shared_ptr<SceneNode>& node,
                       const std::shared_ptr<IShape>& shape, float mass)
    : m_node(node), shape(shape) {
    colShape = std::unique_ptr<btCollisionShape>(shape->createPhysicShape());

    btTransform startTransform;
    startTransform.setIdentity();
    m_mass = mass;
    auto location = node->getLocation();
    startTransform.setOrigin(location.toBtVec3());
    startTransform.setRotation(node->getRotation().toBtQuat());

    myMotionState = std::make_unique<btDefaultMotionState>(startTransform);
}

void PhysObject::update(const std::chrono::milliseconds& dt) {
    btTransform transform;
    physic->getMotionState()->getWorldTransform(transform);
    m_node->setLocation(Vec3f::fromBtVec3(transform.getOrigin()));

    m_node->setRotation(
        Quaternion::fromBtQuat(physic->getWorldTransform().getRotation()));
}

void PhysObject::setPhysic(const std::shared_ptr<btRigidBody>& newBody) {
    physic = newBody;
}

std::shared_ptr<btRigidBody> PhysObject::getPhysics() const {
    return physic;
}

btRigidBody::btRigidBodyConstructionInfo PhysObject::getConstructionInfo() {
    bool isDynamic = (m_mass != 0.f);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic) {
        colShape->calculateLocalInertia(m_mass, localInertia);
    }
    return {m_mass, myMotionState.get(), colShape.get(), localInertia};
}
