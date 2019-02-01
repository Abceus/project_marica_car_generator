#include "object.h"

Object::Object( std::unique_ptr<Mesh>&& model, float x, float y, float z )
        : m_model( std::move( model ) )
        , position( x, y, z )
{

}

Mesh* Object::getModel() const
{
    return this->m_model.get();
}

QVector3D Object::getPosition() const
{
    return this->position;
}
