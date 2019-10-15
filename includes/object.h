#pragma once

#include <memory>

#include "render_system/mesh.h"

class Object
{
public:
    Object( Model model, float x = 0, float y = 0, float z = 0 );
    ~Object() = default;
    Mesh& getModel();
    virtual QVector3D getPosition() const;
protected:
    QVector3D position;
    Mesh m_model;
};
