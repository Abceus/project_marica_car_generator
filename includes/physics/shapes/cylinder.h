#pragma once

#include "physics/shapes/shape.h"

class Cylinder : public IShape
{
public:
    Cylinder( float radius, float height );
    QSharedPointer<Drawable> makeDrawable() const override;
#ifndef WITHOUT_SIMULATION
    QSharedPointer<btCollisionShape> makeBulletShape() const override;
#endif
private:
    float m_radius;
    float m_height;
};
