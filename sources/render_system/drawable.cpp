#include "render_system/drawable.h"
#include "render_system/scene_node.h"

Drawable::Drawable()
    : m_enable( true )
    , m_parent( nullptr )
{

}

void Drawable::setEnable(bool value)
{
    m_enable = value;
}

bool Drawable::getEnable() const
{
    return m_enable;
}

void Drawable::setParent( SceneNode* parent )
{
    m_parent = parent;
}

SceneNode* Drawable::getParent() const
{
    return m_parent;
}

Vector3D Drawable::getLocation()
{
    if(m_parent)
    {
        m_parent->getLocation();
    }
    return Vector3D();
}

Vector3D Drawable::getOriginLocation() const
{
    if(m_parent)
    {
        m_parent->getOriginLocation();
    }
    return Vector3D();
}

Vector3D Drawable::getRotation() const
{
    if(m_parent)
    {
        m_parent->getRotation();
    }
    return Vector3D();
}

Vector3D Drawable::getOriginRotation() const
{
    if(m_parent)
    {
        m_parent->getOriginRotation();
    }
    return Vector3D();
}

Scale3D Drawable::getScale() const
{
    if(m_parent)
    {
        m_parent->getScale();
    }
    return Scale3D();
}

Scale3D Drawable::getOriginScale() const
{
    if(m_parent)
    {
        m_parent->getOriginScale();
    }
    return Scale3D();
}

QMatrix4x4 Drawable::getOriginMatrix() const
{
    if(m_parent)
    {
        m_parent->getOriginMatrix();
    }
    return QMatrix4x4();
}

QMatrix4x4 Drawable::getMatrix() const
{
    if(m_parent)
    {
        m_parent->getMatrix();
    }
    return QMatrix4x4();
}
