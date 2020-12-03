#pragma once

#include "physics/shapes/shape.h"

class Mesh : public IShape
{
public:
    Mesh( ResourcePointer<Model> model );
    QSharedPointer<Drawable> makeDrawable() const override;
#ifndef WITHOUT_SIMULATION
    QSharedPointer<btCollisionShape> makeBulletShape() const override;
#endif
private:
    ResourcePointer<Model> m_model;
};
