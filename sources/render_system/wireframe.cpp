#include "render_system/wireframe.h"

#include <QVector>

#include <algorithm>

struct Edge
{
    GLuint vertexes[2];
};

WireframeMesh::WireframeMesh(Model model)
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

    QVector<Edge> edges;

    for( const auto& indice: model.indices )
    {
        for( size_t i = 0; i < 3; i++ )
        {
            edges.append( { indice.vertexes[i], indice.vertexes[(i+1)%3] } );
        }
    }

    std::sort( edges.begin(), edges.end(), []( const Edge& a, const Edge& b ){ return a.vertexes[0] == b.vertexes[0] ? a.vertexes[1] < b.vertexes[1] : a.vertexes[0] < b.vertexes[0]; } );
    auto removeIterator = std::unique( edges.begin(), edges.end(), []( const Edge& a, const Edge& b ){ return a.vertexes[0] == b.vertexes[0] && a.vertexes[1] == b.vertexes[1]; } );
    edges.erase( removeIterator, edges.end() );

    m_VBO.bind();
    m_VBO.allocate( model.vertices.data(), sizeof( Vertex )*model.vertices.size() );

    m_EBO.bind();
    m_EBO.allocate( edges.data(), sizeof( Edge )*edges.size() );

    f->glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), nullptr );
    f->glEnableVertexAttribArray( 0 );

    m_VAO.release();

    m_VAOsize = edges.size() * 2;

    m_model = model;
}

Model WireframeMesh::getModel()
{
    return m_model;
}

void WireframeMesh::draw(const RenderInfo &renderInfo)
{
    m_VAO.bind();
    renderInfo.f->glDrawElements( GL_LINES, m_VAOsize, GL_UNSIGNED_INT, nullptr );
    m_VAO.release();
}
