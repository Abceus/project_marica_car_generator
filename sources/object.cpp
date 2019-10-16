#include "object.h"

Object::Object( QSharedPointer<Mesh> model, QSharedPointer<SceneNode> node )
        : m_node( node )
        , m_model( model )
{

}

QSharedPointer<SceneNode> Object::getNode()
{
    return m_node;
}

QSharedPointer<Mesh> Object::getDraweable()
{
    return m_model;
}
