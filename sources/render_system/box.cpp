#include "render_system/box.h"

Box::Box( Vector3D size, QColor color )
    : Drawable()
    , m_color( std::move( color ) )
{
    auto halfX = size.x() / 2.0f;
    auto halfY = size.y() / 2.0f;
    auto halfZ = size.z() / 2.0f;

    float vertexes[] = {
                         -halfX, halfY, halfZ,    // 0 near left up
                         -halfX, -halfY, halfZ,     // 1 near left down
                         halfX, halfY, halfZ,     // 2 near right up
                         halfX, -halfY, halfZ,      // 3 near right down
                         -halfX, halfY, -halfZ,     // 4 further left up
                         -halfX, -halfY, -halfZ,      // 5 further left down
                         halfX, halfY, -halfZ,      // 6 further right up
                         halfX, -halfY, -halfZ,       // 7 further right down
                       };

    unsigned indices[] = {
        // near
        0, 2, 1,
        1, 2, 3,
        // further
        4, 5, 6,
        5, 6, 7,
        // left
        4, 0, 5,
        5, 0, 1,
        // right
        2, 3, 7,
        2, 6, 7,
        // down
        1, 3, 7,
        1, 7, 5,
        // up
        0, 2, 6,
        0, 4, 6
    };

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

    m_VBO.bind();
    m_VBO.allocate( vertexes, sizeof( vertexes ) * sizeof( float ) );

    m_EBO.bind();
    m_EBO.allocate( indices, sizeof( indices ) * sizeof( unsigned ) );

    f->glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( float ) * 3, nullptr );
    f->glEnableVertexAttribArray( 0 );

    m_VAO.release();
}

void Box::draw(const RenderInfo &renderInfo)
{
    m_VAO.bind();
    renderInfo.shader->setUniformValue( renderInfo.shader->uniformLocation( "color" ), m_color );
    renderInfo.f->glDrawElements( GL_TRIANGLES, 36, GL_UNSIGNED_INT, nullptr );
    m_VAO.release();
}
