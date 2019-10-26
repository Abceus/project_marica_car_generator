#include "render_system/drawable.h"

void Drawable::subdraw(const RenderInfo &renderInfo, size_t materialIndex, size_t from, size_t size)
{

}

QVector<Face> Drawable::getTransparentFaces() const
{
    return {};
}

QVector<Vertex> Drawable::getVertexes() const
{
    return {};
}

AABBBox Drawable::getRootAABBB() const
{
    return AABBBox();
}
