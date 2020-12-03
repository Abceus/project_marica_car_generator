#pragma once

#include "physics/shapes/shape.h"

class CollectionShape : public IShape
{
public:
    void addChild( QSharedPointer<IShape> shape );
    // TODO: index?
    void removeChild( QSharedPointer<IShape> shape );
    QSharedPointer<Drawable> makeDrawable() const override;
#ifndef WITHOUT_SIMULATION
    QSharedPointer<btCollisionShape> makeBulletShape() const override;
#endif
private:
#ifndef WITHOUT_SIMULATION
    QVector<QSharedPointer<btCollisionShape>> m_btShapes;
#endif
    QVector<QSharedPointer<IShape>> m_shapes;
};
