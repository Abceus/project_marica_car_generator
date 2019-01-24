
#include <includes/object.h>

#include "object.h"

Object::Object(std::unique_ptr<Mesh>&& model, float x, float y, float z) : model(std::move(model)), position(x, y, z)
{

}

Mesh* Object::getModel() const
{
    return this->model.get();
}

QVector3D Object::getPosition() const
{
    return this->position;
}