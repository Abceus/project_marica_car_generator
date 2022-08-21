#include "physics/physobject.h"
#include "utils/math/vec3.h"

PhysObject::PhysObject(const std::shared_ptr<SceneNode>& node,
                       const Model& physModel, float mass, Vec3f size) {
    m_node = node;
    colShape = std::make_unique<btConvexHullShape>();

    auto verts = physModel.vertices;
    for (auto& vert : verts) {
        static_cast<btConvexHullShape*>(colShape.get())
            ->addPoint(btVector3(vert.X, vert.Y, vert.Z));
    }
    btTransform startTransform;
    startTransform.setIdentity();
    m_mass = mass;
    auto location = node->getLocation();
    startTransform.setOrigin(
        btVector3(location.getX(), location.getY(), location.getZ()));
    // auto rotationQuaternion = QQuaternion::fromEulerAngles(node->getRotation());
    // startTransform.setRotation(
    //     btQuaternion(rotationQuaternion.x(), rotationQuaternion.y(),
    //                  rotationQuaternion.z(), rotationQuaternion.length()));

    myMotionState = std::make_unique<btDefaultMotionState>(startTransform);
}

void PhysObject::update(const std::chrono::milliseconds& dt) {
    btTransform transform;
    physic->getMotionState()->getWorldTransform(transform);
    m_node->setLocation(Vec3f(transform.getOrigin().x(),
                                  transform.getOrigin().y(),
                                  transform.getOrigin().z()));

    // btQuaternion rotation = physic->getWorldTransform().getRotation();
    // m_node->setRotation( QQuaternion( rotation.w(), rotation.x(),
    // rotation.y(), rotation.z() ).toEulerAngles() );
}

void PhysObject::setPhysic(const std::shared_ptr<btRigidBody>& newBody) {
    physic = newBody;
    //    physic->getWorldTransform().setOrigin(btVector3(position.x(),
    //    position.y(), position.z()));
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
