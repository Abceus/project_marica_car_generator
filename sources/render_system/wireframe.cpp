#include "render_system/wireframe.h"

#include <QVector>

#include <algorithm>


WireframeMesh::WireframeMesh(const WireframeModel& model, QColor color)
    : Drawable()
    , m_color( std::move( color ) )
{
    DrawBuffer db;
    for(const auto& vertex: model.vertices)
    {
        DrawBuffer::Vertex newVertex;
        newVertex.coords.setX(vertex.X);
        newVertex.coords.setY(vertex.Y);
        newVertex.coords.setZ(vertex.Z);
        db.vertices.push_back(newVertex);
    }

    for(const auto& edge: model.edges)
    {
        db.indices.push_back(edge.vertexes[0]);
        db.indices.push_back(edge.vertexes[1]);
    }

    db.drawType = DrawType::DT_Overlay;
    db.elementType = ElementsType::PT_Line;
    db.material.color = color;
    m_submeshes.push_back(db);
}

DrawBuffers WireframeMesh::getDrawBuffers() const
{
    return m_submeshes;
}
