#pragma once

#include "physics/shapes/shape.h"

class Sphere : public IShape
{
public:
    Sphere( float radius );
    QSharedPointer<Drawable> makeDrawable() const override;
#ifndef WITHOUT_SIMULATION
    QSharedPointer<btCollisionShape> makeBulletShape() const override;
#endif
private:
    float m_radius;
};
