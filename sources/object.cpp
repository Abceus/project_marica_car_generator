#include "object.h"

Object::Object( QSharedPointer<Drawable> model, QSharedPointer<SceneNode> node )
        : m_node( node )
        , m_drawable( model )
{

}

QSharedPointer<SceneNode> Object::getNode()
{
    return m_node;
}

QSharedPointer<Drawable> Object::getDrawable()
{
    return m_drawable;
}

void Object::setNode(QSharedPointer<SceneNode> node)
{
    m_node = node;
}

void Object::setDrawable(QSharedPointer<Drawable> drawable)
{
    m_drawable = drawable;
}
