#include "physics/physobject.h"
#include "utils/math/quaternion.h"
#include "utils/math/rot3.h"
#include "utils/math/vec3.h"
#include "wx/log.h"

PhysObject::PhysObject(const std::shared_ptr<SceneNode>& node,
                       const Model& physModel, float mass, Vec3f size) {
    m_node = node;
    colShape = std::make_unique<btConvexHullShape>();

    auto verts = physModel.vertices;
    for (auto& vert : verts) {
        static_cast<btConvexHullShape*>(colShape.get())
            ->addPoint(Vec3f(vert.X, vert.Y, vert.Z).toBtVec3());
    }
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

    wxLogDebug((std::string("x: ") + std::to_string(Vec3f::fromBtVec3(transform.getOrigin()).getX())).c_str());
    wxLogDebug((std::string("y: ") + std::to_string(Vec3f::fromBtVec3(transform.getOrigin()).getY())).c_str());
    wxLogDebug((std::string("z: ") + std::to_string(Vec3f::fromBtVec3(transform.getOrigin()).getZ())).c_str());

    btQuaternion rotation = physic->getWorldTransform().getRotation();
    // btScalar yawZ;
    // btScalar pitchY;
    // btScalar rollX;
    // rotation.getEulerZYX(yawZ, pitchY, rollX);
    // Rotor3 rot;
    // rot.setPitch(Angle::fromRadians(-yawZ));
    // rot.setYaw(Angle::fromRadians(rollX));
    // rot.setRoll(Angle::fromRadians(pitchY));
    m_node->setRotation(Quaternion::fromBtQuat(rotation));
}

void PhysObject::setPhysic(const std::shared_ptr<btRigidBody>& newBody) {
    physic = newBody;
    //    physic->getWorldTransform().setOrigin(btVector3(position.x(),
    //    position.y(), position.z()));
}

std::shared_ptr<btRigidBody> PhysObject::getPhysics() const {
    return physic;
}

// QVector3D PhysObject::getPosition() const
//{
//     btTransform transform;
//     physic->getMotionState()->getWorldTransform( transform );
//     return QVector3D( transform.getOrigin().x(), transform.getOrigin().y(),
//     transform.getOrigin().z() );
// }

// QQuaternion PhysObject::getRotation() const {
//     btQuaternion rotation = physic->getWorldTransform().getRotation();
//     return {rotation.w(), rotation.x(), rotation.y(), rotation.z()};
// }

btRigidBody::btRigidBodyConstructionInfo PhysObject::getConstructionInfo() {
    bool isDynamic = (m_mass != 0.f);
    btVector3 localInertia(0, 0, 0);
    if (isDynamic) {
        colShape->calculateLocalInertia(m_mass, localInertia);
    }
    return {m_mass, myMotionState.get(), colShape.get(), localInertia};
}
