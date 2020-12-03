#pragma once

#include "physics/shapes/shape.h"

class Offset : public IShape
{
public:
    Offset( QSharedPointer<IShape> shape, const Vector3D& position, const Vector3D& rotation, const Scale3D& scale );
    QSharedPointer<Drawable> makeDrawable() const override;
#ifndef WITHOUT_SIMULATION
    QSharedPointer<btCollisionShape> makeBulletShape() const override;
#endif
private:
    QSharedPointer<IShape> m_shape;
    Vector3D m_position;
    Vector3D m_rotation;
    Scale3D m_scale;
#ifndef WITHOUT_SIMULATION
    QSharedPointer<btCollisionShape> m_bulletShape;
#endif
};
