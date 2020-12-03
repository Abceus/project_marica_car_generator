#pragma once

#include "physics/shapes/shape.h"

class Box : public IShape
{
public:
    Box( float xExtend, float yExtend, float zExtend );
    QSharedPointer<Drawable> makeDrawable() const override;
#ifndef WITHOUT_SIMULATION
    QSharedPointer<btCollisionShape> makeBulletShape() const override;
#endif
private:
    float m_xExtend, m_yExtend, m_zExtend;
};
