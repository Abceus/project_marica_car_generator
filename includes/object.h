#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include "model.h"

class Object
{
private:
    float x, y, z;
    std::unique_ptr<Model> model;
public:
    Object(std::unique_ptr<Model> model, float x, float y, float z);
    ~Object() = default;
    Model* getModel() const;
    float getX();
    float getY();
    float getZ();
};

#endif // OBJECT_H
