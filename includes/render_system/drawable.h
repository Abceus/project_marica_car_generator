#pragma once

#include "render_system/render_info.h"
#include <QVector>
#include <QVector3D>

struct Face
{
    size_t indiceIndex;
    QVector3D points[3];
};

struct Subdrawable
{
    QVector<Face> faces;
};

class Drawable
{
public:
    virtual void draw( const RenderInfo& renderInfo ) = 0;
    virtual QVector<Subdrawable> getTransparentSubdrawables() const;
};
