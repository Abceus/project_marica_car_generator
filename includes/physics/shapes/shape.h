#pragma once

#include "render_system/drawable.h"

#ifndef WITHOUT_SIMULATION
#include <btBulletDynamicsCommon.h>
#endif

class IShape
{
public:
    // TODO: make to get?
    virtual QSharedPointer<Drawable> makeDrawable() const = 0;
#ifndef WITHOUT_SIMULATION
    virtual QSharedPointer<btCollisionShape> makeBulletShape() const = 0;
#endif
};
