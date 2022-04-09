#include <memory>
#include <algorithm>

#include <QFile>

#include "render_system/drawable.h"
#include "resources/resource_manager.h"
#include "render_system/mesh.h"
#include "render_system/renderer.h"

Mesh::Mesh()
        : Drawable()
        , VAOsize( 0u )
{
}

Mesh::Mesh( const Model& model )
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions* fe = QOpenGLContext::currentContext()->extraFunctions();
    if(!f || !fe)
        return;

    VAO.create();
    VBO = QOpenGLBuffer( QOpenGLBuffer::VertexBuffer );
    VBO.create();
    VBO.setUsagePattern( QOpenGLBuffer::StaticDraw );
    EBO = QOpenGLBuffer( QOpenGLBuffer::IndexBuffer );
    EBO.create();
    EBO.setUsagePattern( QOpenGLBuffer::StaticDraw );

    VAO.bind();

    VBO.bind();
    VBO.allocate( model.vertices.data(), sizeof( Vertex )*model.vertices.size() );

    EBO.bind();
    EBO.allocate( model.indices.data(), sizeof( Indice )*model.indices.size() );

    f->glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), nullptr );
    f->glEnableVertexAttribArray( 0 );
    f->glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ),
            reinterpret_cast<GLvoid*>( offsetof( Vertex, U ) ) );
    f->glEnableVertexAttribArray( 1 );
    fe->glVertexAttribIPointer( 2, 1, GL_INT, sizeof( Vertex ),
            reinterpret_cast<GLvoid*>( offsetof( Vertex, MaterialIndex ) ) );
    fe->glEnableVertexAttribArray( 2 );

    VAO.release();

    VAOsize = model.VAOsize;

    for( auto& material: model.materials )
    {
        auto image = ResourceManager::Instance().get<QString, QImage>( material );
        if(image->isNull()) 
        {
            image = ResourceManager::Instance().get<QString, QImage>( "./resources/textures/test.jpg" );
        }
        addTexture(Renderer::getCurrentRenderer()->makeDrawable<QOpenGLTexture>(*image));
    }

    m_model = model;
}

Mesh::~Mesh()
{
    VAO.destroy();
    VBO.destroy();
    EBO.destroy();
}

GLsizei Mesh::getVAOsize()
{
    return this->VAOsize;
}

size_t Mesh::getTexturesSize()
{
    return this->textures.size();
}

void Mesh::setTexture( const QSharedPointer<QOpenGLTexture>& texture, size_t index )
{
    // setTextureQueue( index, averageAlpha( image ) );
    setTextureQueue( index, 0 );
    textures[index] = texture;
    sortTextures();
}

void Mesh::addTexture( const QSharedPointer<QOpenGLTexture>& texture )
{
    // textureQueue.emplace_back( textures.size(), averageAlpha( image ) );
    textureQueue.emplace_back( textures.size(), 0 );
    textures.append( texture );
    sortTextures();
}

void Mesh::sortTextures()
{
    std::sort( this->textureQueue.begin(), this->textureQueue.end(),
                  []( const std::pair<int, float>& lhs, const std::pair<int, float>& rhs)
                  {
                        return lhs.second > rhs.second;
                  }
              );
}

size_t Mesh::getTextureQueue( size_t index )
{
    return this->textureQueue.at( index ).first;
}

float Mesh::averageAlpha( const QSharedPointer<QImage>& image )
{
    float sum = 0;
    for( int i=0; i<image->height(); i++ )
    {
        for( int j=0; j<image->width(); j++ )
        {
            sum += image->pixelColor( j, i ).alpha();
        }
    }
    return sum / ( image->height() * image->width() );
}

void Mesh::setTextureQueue( size_t index, float average )
{
    for( auto& texture: textureQueue )
    {
        if( texture.first == index )
        {
            texture.second = average;
            return;
        }
    }
}

void Mesh::bindVAO()
{
    VAO.bind();
}

void Mesh::releaseVAO()
{
    VAO.release();
}

void Mesh::bindTexture( size_t index )
{
    textures.at( index )->bind();
}

Model Mesh::getModel()
{
    return m_model;
}

void Mesh::draw( const RenderInfo& renderInfo )
{
    bindVAO();
    for( size_t i=0; i<getTexturesSize(); i++ )
    {
        if(textures.at( i )) 
        {
            size_t index = getTextureQueue( i );
            bindTexture( i );
            renderInfo.shader->setUniformValue( "texture", 0 );
            renderInfo.shader->setUniformValue( renderInfo.shader->uniformLocation( "nowTexture" ), static_cast<GLint>( index ) );
            renderInfo.f->glDrawElements( GL_TRIANGLES, getVAOsize(), GL_UNSIGNED_INT, nullptr );
        }
    }
    releaseVAO();
}
