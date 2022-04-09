#include "object.h"

Object::Object( QSharedPointer<Drawable> model, QSharedPointer<SceneNode> node )
        : m_node( node )
        , m_model( model )
{

}

QSharedPointer<SceneNode> Object::getNode()
{
    return m_node;
}

QSharedPointer<Drawable> Object::getDrawable()
{
    return m_model;
}

void Object::setDrawable(const QSharedPointer<Drawable>& newModel) 
{
    m_model = newModel;
}