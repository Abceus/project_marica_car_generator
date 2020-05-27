#include "render_system/wireframe.h"

#include <QVector>

#include <algorithm>

struct Edge
{
    GLuint vertexes[2];
};

WireframeMesh::WireframeMesh(ResourcePointer<Model> model, QColor color)
    : Drawable()
    , m_color( std::move( color ) )
{
    QVector<Vertex> vertices;

    for( const auto& vertex: model->vertices )
    {
        vertices.append( { vertex.Y, vertex.Z, -vertex.X, vertex.U, vertex.V, vertex.MaterialIndex } );
    }

    std::sort( vertices.begin(), vertices.end(), []( const Vertex& a, const Vertex& b )
    {
        if( a.X != b.X )
        {
            return a.X < b.X;
        }
        if( a.Y != b.Y )
        {
            return a.Y < b.Y;
        }
        return a.Z < b.Z;
    } );

    auto vertexRemoveIterator = std::unique( vertices.begin(), vertices.end(), []( const Vertex& a, const Vertex& b ){ return a.X == b.X && a.Y == b.Y && a.Z == b.Z; } );
    vertices.erase( vertexRemoveIterator, vertices.end() );

    QVector<Edge> edges;

    QVector<GLuint> vertexConformity;
    vertexConformity.reserve( model->vertices.size());

    for( const auto& vertex: model->vertices )
    {
        vertexConformity.append( static_cast<GLuint>( std::find_if( vertices.begin(), vertices.end(),
                                               [vertex]( const Vertex& a )
                                                  {
                                                        return a.X == vertex.Y && a.Y == vertex.Z && a.Z == -vertex.X;
//                                                        return a.X == vertex.X && a.Y == vertex.Y && a.Z == vertex.Z;
                                                  }) - vertices.begin() ) );
    }

    for( const auto& indice: model->indices )
    {
        for( size_t i = 0; i < 3; i++ )
        {
            auto firstPoint = vertexConformity[indice.vertexes[i]];
            auto secondPoint = vertexConformity[indice.vertexes[(i+1)%3]];
            auto min = std::min( firstPoint, secondPoint );
            auto max = std::max( firstPoint, secondPoint );
            edges.append( { min, max } );
        }
    }

    std::sort( edges.begin(), edges.end(), []( const Edge& a, const Edge& b ){ return a.vertexes[0] == b.vertexes[0] ? a.vertexes[1] < b.vertexes[1] : a.vertexes[0] < b.vertexes[0]; } );
    auto edgeRemoveIterator = std::unique( edges.begin(), edges.end(), []( const Edge& a, const Edge& b ){ return a.vertexes[0] == b.vertexes[0] && a.vertexes[1] == b.vertexes[1]; } );
    edges.erase( edgeRemoveIterator, edges.end() );
}

QVector<DrawBuffer> WireframeMesh::getDrawBuffer() const
{
    return QVector<DrawBuffer>();
}
