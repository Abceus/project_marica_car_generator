#include "physics/shapes/collection.h"
#include "render_system/scene_node.h"

void CollectionShape::addChild( QSharedPointer<IShape> shape )
{
    m_shapes.push_back(shape);
    m_btShapes.push_back(shape->makeBulletShape());
}

void CollectionShape::removeChild( QSharedPointer<IShape> shape )
{
    auto index = m_shapes.indexOf(shape);
    m_shapes.remove(index);
    m_btShapes.remove(index);
}

QSharedPointer<Drawable> CollectionShape::makeDrawable() const
{
    QSharedPointer<SceneNode> node(new SceneNode);
    for(auto& shape: m_shapes)
    {
        node->addChild(shape->makeDrawable());
    }
    return node;
}

#ifndef WITHOUT_SIMULATION
QSharedPointer<btCollisionShape> CollectionShape::makeBulletShape() const
{
    QSharedPointer<btCompoundShape> result;
    for(auto& shape: m_btShapes)
    {
        result->addChildShape(btTransform(), shape.get());
    }
    return result;
}
#endif
