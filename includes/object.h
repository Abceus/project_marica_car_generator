#pragma once

#include <memory>

#include "mesh.h"

class Object
{
private:
    QVector3D position;
    std::unique_ptr<Mesh> model;
public:
    Object( std::unique_ptr<Mesh>&& model, float x = 0, float y = 0, float z = 0 );
    ~Object() = default;
    Mesh* getModel() const;
    QVector3D getPosition() const;
};
