#pragma once

#include "render_system/render_info.h"
#include "resources/model.h"
#include <QVector>
#include <QVector3D>

struct Face
{
    size_t indiceIndex;
    size_t materialIndex;
    size_t points[3];
};

class Drawable
{
public:
    virtual void draw( const RenderInfo& renderInfo ) = 0;
    // TODO: rework for transparent faces
    virtual void subdraw( const RenderInfo& renderInfo, size_t materialIndex, size_t from, size_t size );
    virtual QVector<Face> getTransparentFaces() const;
    virtual QVector<Vertex> getVertexes() const;
};
