#include "render_system/scene_node.h"
#include "render_system/batching.h"

#include <algorithm>
#include <QtMath>

SceneNode::SceneNode()
    : Drawable()
    , m_location()
    , m_rotation()
    , m_scale( 1.0f, 1.0f, 1.0f )
    , m_needMatrixUpdate(true)
    , m_needBufferUpdate(true)
{

}

void SceneNode::updateMatrix()
{
    m_needMatrixUpdate = true;
    for(const auto& child: m_childrens)
    {
        auto sceneNodeChild = child.staticCast<SceneNode>();
        if(sceneNodeChild)
        {
            sceneNodeChild->updateMatrix();
        }
    }
}

void SceneNode::updateBuffer()
{
    m_needBufferUpdate = true;
    if(m_parent)
    {
        m_parent->updateBuffer();
    }
}

Vector3D SceneNode::getLocation()
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
    updateMatrix();
}

Vector3D SceneNode::getRotation() const
{
    auto parentNode = static_cast<SceneNode*>(m_parent);
    if( parentNode )
    {
        return m_rotation + parentNode->getRotation();
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
    updateMatrix();
}

Scale3D SceneNode::getScale() const
{
    auto parentNode = static_cast<SceneNode*>(m_parent);
    if( parentNode )
    {
        return m_scale * parentNode->getScale();
    }
    return m_scale;
}

Scale3D SceneNode::getOriginScale() const
{
    return m_scale;
}

void SceneNode::setScale(const Scale3D &scale)
{
    m_scale = scale;
    updateMatrix();
}

void SceneNode::setScale(float scale)
{
    m_scale = { scale, scale, scale };
    updateMatrix();
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
    updateMatrix();
}

QSharedPointer<Drawable> SceneNode::addChild(QSharedPointer<Drawable> newChild)
{
    if( !m_childrens.contains(newChild) )
    {
        m_childrens.append(newChild);
        newChild->setParent(this);
        updateBuffer();
    }
    return newChild;
}

void SceneNode::removeChild(Drawable *removeChild)
{
    auto found = std::find(m_childrens.begin(), m_childrens.end(), removeChild);
    if( found != m_childrens.end() )
    {
        m_childrens.erase( found );
        removeChild->setParent(nullptr);
    }
    updateBuffer();
}

void SceneNode::removeChild( QSharedPointer<Drawable> removedChild )
{
    removeChild(removedChild.get());
}

bool SceneNode::isEmpty() const
{
    return m_childrens.empty();
}

void SceneNode::clear()
{
    for(auto rbeginIt = m_childrens.rbegin(); rbeginIt != m_childrens.rend(); rbeginIt++)
    {
        removeChild(rbeginIt->get());
    }
}

QVector<QSharedPointer<Drawable>>::ConstIterator SceneNode::begin()
{
    return m_childrens.begin();
}

QVector<QSharedPointer<Drawable>>::ConstIterator SceneNode::end()
{
    return m_childrens.end();
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
    if( m_needMatrixUpdate )
    {
        if( m_parent )
        {
            m_cachedMatrix = m_parent->getMatrix() * getOriginMatrix();
        }
        else
        {
            m_cachedMatrix = getOriginMatrix();
        }
        m_needMatrixUpdate = false;
    }
    return m_cachedMatrix;
}

DrawBuffers SceneNode::getDrawBuffers() const
{
    if( m_needBufferUpdate )
    {
        m_cachedBuffer.clear();
        for(const auto& node: m_childrens)
        {
            m_cachedBuffer = m_cachedBuffer + node->getDrawBuffers() * getOriginMatrix();
        }
        m_needBufferUpdate = false;
    }

    return m_cachedBuffer;
}
