#include "render_system/wireframe.h"

#include <QVector>

#include <algorithm>

struct Edge
{
    GLuint vertexes[2];
};

WireframeMesh::WireframeMesh(Model model, QColor color)
    : Drawable()
    , m_model( std::move( model ) )
    , m_color( std::move( color ) )
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions* fe = QOpenGLContext::currentContext()->extraFunctions();
    if(!f || !fe)
        return;

    m_VAO.create();
    m_VBO = QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
    m_VBO.create();
    m_VBO.setUsagePattern( QOpenGLBuffer::StaticDraw );
    m_EBO = QOpenGLBuffer( QOpenGLBuffer::IndexBuffer );
    m_EBO.create();
    m_EBO.setUsagePattern( QOpenGLBuffer::StaticDraw );

    m_VAO.bind();

    QVector<Vertex> vertices;

    for( const auto& vertex: m_model.vertices )
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
    vertexConformity.reserve( m_model.vertices.size());

    for( const auto& vertex: m_model.vertices )
    {
        vertexConformity.append( static_cast<GLuint>( std::find_if( vertices.begin(), vertices.end(),
                                               [vertex]( const Vertex& a )
                                                  {
                                                        return a.X == vertex.Y && a.Y == vertex.Z && a.Z == -vertex.X;
//                                                        return a.X == vertex.X && a.Y == vertex.Y && a.Z == vertex.Z;
                                                  }) - vertices.begin() ) );
    }

    for( const auto& indice: m_model.indices )
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

    m_VBO.bind();
    m_VBO.allocate( vertices.data(), sizeof( Vertex )*vertices.size() );

    m_EBO.bind();
    m_EBO.allocate( edges.data(), sizeof( Edge )*edges.size() );

    f->glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), nullptr );
    f->glEnableVertexAttribArray( 0 );

    m_VAO.release();

    m_VAOsize = edges.size() * 2;
}

Model WireframeMesh::getModel()
{
    return m_model;
}

void WireframeMesh::draw(const RenderInfo &renderInfo)
{
    m_VAO.bind();
    renderInfo.shader->setUniformValue( renderInfo.shader->uniformLocation( "color" ), m_color );
    renderInfo.f->glDrawElements( GL_LINES, m_VAOsize, GL_UNSIGNED_INT, nullptr );
    m_VAO.release();
}
