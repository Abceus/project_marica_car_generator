#ifndef OBJECT_H
#define OBJECT_H

#include <memory>

#include "model.h"

class Object
{
private:
    QVector3D position;
    std::unique_ptr<Model> model;
public:
    Object(std::unique_ptr<Model>&& model, float x = 0, float y = 0, float z = 0);
    ~Object() = default;
    Model* getModel() const;
    QVector3D getPosition() const;
};

#endif // OBJECT_H
