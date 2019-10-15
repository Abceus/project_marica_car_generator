#include "object.h"

Object::Object( Model model, float x, float y, float z )
        : m_model( model )
        , position( x, y, z )
{

}

Mesh &Object::getModel()
{
    return m_model;
}

QVector3D Object::getPosition() const
{
    return this->position;
}
