#include "object.h"

Object::Object(std::unique_ptr<Model> model, float x, float y, float z) : model(std::move(model)), x(x), y(y), z(z)
{

}

Object::~Object()
{

}

Model* Object::getModel()
{
    return this->model.get();
}

float Object::getX()
{
    return this->x;
}

float Object::getY()
{
    return this->y;
}

float Object::getZ()
{
    return this->z;
}
