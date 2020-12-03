#include "physics/shapes/offset.h"
#include "render_system/wireframe.h"
#include "render_system/scene_node.h"

#include <QtMath>

Offset::Offset( QSharedPointer<IShape> shape, const Vector3D& position, const Vector3D& rotation, const Scale3D& scale )
    : m_shape( shape )
    , m_position( position )
    , m_rotation( rotation )
    , m_scale( scale )
{
    m_bulletShape = m_shape->makeBulletShape();
}

QSharedPointer<Drawable> Offset::makeDrawable() const
{
    QSharedPointer<SceneNode> node(new SceneNode());
    node->setScale( m_scale );
    node->setLocation( m_position );
    node->setRotation( m_rotation );
    node->addChild( m_shape->makeDrawable() );
    return node;
}

#ifndef WITHOUT_SIMULATION
QSharedPointer<btCollisionShape> Offset::makeBulletShape() const
{
    QSharedPointer<btCompoundShape> result(new btCompoundShape());
    btQuaternion rotation;
    auto rotationEuler = m_rotation.getBulletVector();
    rotation.setEuler(rotationEuler.x(), rotationEuler.y(), rotationEuler.z());
    result->addChildShape(btTransform(rotation, m_position.getBulletVector()), m_bulletShape.get());
    result->setLocalScaling(m_scale.getBulletVector());
    return result;
}
#endif
