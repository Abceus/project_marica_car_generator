#pragma once

#include "render_system/render_info.h"
#include "resources/model.h"
#include "render_system/aabb.h"
#include "render_system/face.h"
#include <QVector>

class Drawable
{
public:
    Drawable();
    virtual void draw( const RenderInfo& renderInfo ) = 0;
    void setEnable( bool value );
    bool getEnable() const;
    // TODO: rework for transparent faces
    virtual void subdraw( const RenderInfo& renderInfo, size_t materialIndex, size_t from, size_t size );
    virtual QVector<Face> getTransparentFaces() const;
    virtual QVector<Vertex> getVertexes() const;
    virtual AABBBox getRootAABBB() const;
protected:
    bool m_enable;
};
