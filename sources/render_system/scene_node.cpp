#include "render_system/scene_node.h"

#include <algorithm>

SceneNode::SceneNode()
    : m_location()
    , m_rotation()
    , m_scale( 1.0f, 1.0f, 1.0f )
    , m_parent(nullptr)
    , m_drawables()
{

}

QVector3D SceneNode::getLocation() const
{
    if( m_parent )
    {
        return m_location + m_parent->getLocation();
    }
    return m_location;
}

void SceneNode::setLocation(const QVector3D &location)
{
    m_location = location;
}

QVector3D SceneNode::getRotation() const
{
    if( m_parent )
    {
        return m_rotation + m_parent->getRotation();
    }
    return m_rotation;
}

void SceneNode::setRotation(const QVector3D &rotation)
{
    m_rotation = rotation;
}

QVector3D SceneNode::getScale() const
{
    if( m_parent )
    {
        return m_scale * m_parent->getScale();
    }
    return m_scale;
}

void SceneNode::setScale(const QVector3D &scale)
{
    m_scale = scale;
}

void SceneNode::setScale(float scale)
{
    m_scale = { scale, scale, scale };
}

void SceneNode::setParent(SceneNode *parent)
{
    if( m_parent == parent )
    {
        return;
    }

    if( m_parent )
    {
        m_parent->removeChild(this);
    }
    m_parent = parent;
}

QSharedPointer<SceneNode> SceneNode::addChild(QSharedPointer<SceneNode> newChild)
{
    if( !m_childrens.contains(newChild) )
    {
        m_childrens.append(newChild);
        newChild->setParent(this);
    }
    return newChild;
}

void SceneNode::removeChild(SceneNode *removeChild)
{
    auto found = std::find(m_childrens.begin(), m_childrens.end(), removeChild);
    if( found != m_childrens.end() )
    {
        m_childrens.erase( found );
        removeChild->setParent(nullptr);
    }
}

QSharedPointer<Drawable> SceneNode::addDrawable(QSharedPointer<Drawable> newDrawable)
{
    if( !m_drawables.contains(newDrawable) )
    {
        m_drawables.append(newDrawable);
    }
    return newDrawable;
}

void SceneNode::removeDrawable(Drawable *removeDrawable)
{
    auto found = std::find( m_drawables.begin(), m_drawables.end(), removeDrawable );
    if( found != m_drawables.end() )
    {
        m_drawables.erase(found);
    }
}

bool SceneNode::isEmpty() const
{
    return m_childrens.empty();
}

void SceneNode::clear()
{
    m_childrens.clear();
    m_drawables.clear();
}

QVector<QSharedPointer<SceneNode>>::ConstIterator SceneNode::begin()
{
    return m_childrens.begin();
}

QVector<QSharedPointer<SceneNode>>::ConstIterator SceneNode::end()
{
    return m_childrens.end();
}

QVector<QSharedPointer<Drawable>>::ConstIterator SceneNode::drawableBegin()
{
    return m_drawables.begin();
}

QVector<QSharedPointer<Drawable>>::ConstIterator SceneNode::drawableEnd()
{
    return m_drawables.end();
}
