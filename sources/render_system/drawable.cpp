#include "render_system/drawable.h"

Drawable::Drawable()
    : m_enable( true )
{

}

void Drawable::setEnable(bool value)
{
    m_enable = value;
}

bool Drawable::getEnable() const
{
    return m_enable;
}

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
