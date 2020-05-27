#include "render_system/scene_node.h"

#include <algorithm>
#include <QtMath>

SceneNode::SceneNode()
    : m_location()
    , m_rotation()
    , m_scale( 1.0f, 1.0f, 1.0f )
    , m_parent(nullptr)
    , m_drawables()
{

}

Vector3D SceneNode::getLocation() const
{
    return getMatrix() * QVector3D();
}

Vector3D SceneNode::getOriginLocation() const
{
    return m_location;
}

void SceneNode::setLocation(const Vector3D &location)
{
    m_location = location;
}

Vector3D SceneNode::getRotation() const
{
    if( m_parent )
    {
        return m_rotation + m_parent->getRotation();
    }
    return m_rotation;
}

Vector3D SceneNode::getOriginRotation() const
{
    return m_rotation;
}

//Vector3D SceneNode::getParentRotation() const
//{
//    if( m_parent )
//    {
//        return m_parent->getRotation();
//    }
//    return { 0.0f, 0.0f, 0.0f };
//}

void SceneNode::setRotation(const Vector3D &rotation)
{
    m_rotation = rotation;
}

Scale3D SceneNode::getScale() const
{
    if( m_parent )
    {
        return m_scale * m_parent->getScale();
    }
    return m_scale;
}

void SceneNode::setScale(const Scale3D &scale)
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

void SceneNode::removeChild( QSharedPointer<SceneNode> removedChild )
{
    removeChild(removedChild.get());
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
    if( !m_drawables.empty() )
    {
        return false;
    }

    for( const auto& child: m_childrens )
    {
        if( !child->isEmpty() )
        {
            return false;
        }
    }
    return true;
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

QMatrix4x4 SceneNode::getOriginMatrix() const
{
    QMatrix4x4 result;
    result.translate( m_location.getQtVector() );
    result.rotate( QQuaternion::fromEulerAngles( m_rotation.getQtVector() ) );
    result.scale( m_scale.getQtVector() );
    return result;
}

QMatrix4x4 SceneNode::getMatrix() const
{
    if( m_parent )
    {
        return getOriginMatrix() * m_parent->getMatrix();
    }
    return getOriginMatrix();
}
