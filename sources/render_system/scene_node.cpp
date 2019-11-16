#include "render_system/scene_node.h"

#include <algorithm>
#include <QtMath>

SceneNode::SceneNode()
    : m_location()
    , m_rotation()
    , m_scale( 1.0f, 1.0f, 1.0f )
    , m_parent(nullptr)
    , m_drawables()
    , m_shaderProgram(nullptr)
{

}

Vector3D SceneNode::getLocation() const
{
    if( m_parent )
    {
        return m_location + m_parent->getLocation();
    }
    return m_location;
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

Vector3D SceneNode::getParentRotation() const
{
    if( m_parent )
    {
        return m_parent->getRotation();
    }
    return { 0.0f, 0.0f, 0.0f };
}

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

//    if( m_parent )
//    {
//        m_parent->removeChild(this);
//    }
    m_parent = parent;
}

QSharedPointer<SceneNode> SceneNode::addChild(QSharedPointer<SceneNode> newChild)
{
    if( !m_childrens.contains(newChild) )
    {
        m_childrens.append(newChild);
        newChild->setParent(this);
        connect( newChild.get(), &SceneNode::drawableAdded, this, [=](SceneNode* node, Drawable* drawable){ emit drawableAdded(node, drawable); });
        connect( newChild.get(), &SceneNode::drawableRemoved, this, [=](SceneNode* node, Drawable* drawable){ emit drawableRemoved(node, drawable); });
        connect( newChild.get(), &SceneNode::nodeAdded, this, [=](SceneNode* node){ emit nodeAdded(node); });
        connect( newChild.get(), &SceneNode::nodeRemoved, this, [=](SceneNode* node){ emit nodeRemoved(node); });
        emit nodeAdded( newChild.get() );
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
        emit nodeRemoved( removeChild );
    }
}

QSharedPointer<Drawable> SceneNode::addDrawable(QSharedPointer<Drawable> newDrawable)
{
    if( !m_drawables.contains(newDrawable) )
    {
        m_drawables.append(newDrawable);
        emit drawableAdded( this, newDrawable.get() );
    }
    return newDrawable;
}

void SceneNode::removeDrawable(Drawable *removeDrawable)
{
    auto found = std::find( m_drawables.begin(), m_drawables.end(), removeDrawable );
    if( found != m_drawables.end() )
    {
        m_drawables.erase(found);
        emit drawableRemoved( this, removeDrawable );
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

void SceneNode::setShaderProgram(QSharedPointer<QOpenGLShaderProgram> newProgram)
{
    m_shaderProgram = newProgram;
}

QSharedPointer<QOpenGLShaderProgram> SceneNode::getShaderProgram() const
{
    return m_shaderProgram;
}

QMatrix4x4 SceneNode::getMatrix() const
{
    QMatrix4x4 result;
    result.translate( getLocation().getQtVector() );
    result.rotate( QQuaternion::fromEulerAngles( getRotation().getQtVector() ) );
    result.scale( getScale().getQtVector() );
    return result;
}
