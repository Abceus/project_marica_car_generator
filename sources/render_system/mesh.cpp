#include <memory>
#include <algorithm>

#include <QFile>

#include "resources/resource_manager.h"
#include "render_system/mesh.h"

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
        addTexture( material );
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

void Mesh::setTexture( QString filename, size_t index )
{
    QImage image = ResourceManager::Instance().get<QString, QImage>( filename );
    if( !image.isNull() )
    {
        this->setTextureQueue( index, this->averageAlpha( image ) );
        this->textures[index]->setData( image );
    }
    else
    {
        //TODO: Load default image from texture manager
        image = ResourceManager::Instance().get<QString, QImage>( "./resources/textures/test.jpg" );
        this->setTextureQueue( index, this->averageAlpha( image ) );
        this->textures[index]->setData( image );
    }
    this->sortTextures();
}

void Mesh::addTexture( QString filename )
{
    QImage image = ResourceManager::Instance().get<QString, QImage>( filename );
    if( !image.isNull() )
    {
        this->textureQueue.emplace_back( this->textures.size(), this->averageAlpha( image ) );
//        this->textures.emplace_back( QOpenGLTexture( image ) );
    }
    else
    {
        //TODO: Load default image from texture manager
        image = ResourceManager::Instance().get<QString, QImage>( "./resources/textures/test.jpg" );
        this->textureQueue.emplace_back( this->textures.size(), this->averageAlpha( image ) );
        this->textures.append( QSharedPointer<QOpenGLTexture>( new QOpenGLTexture( image ) ) );
    }
    this->sortTextures();
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

float Mesh::averageAlpha( QImage image )
{
    float sum = 0;
    for( int i=0; i<image.height(); i++ )
    {
        for( int j=0; j<image.width(); j++ )
        {
            sum += image.pixelColor( j, i ).alpha();
        }
    }
    return sum / ( image.height() * image.width() );
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
    this->textures.at( index )->bind();
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
        size_t index = getTextureQueue( i );
        bindTexture( i );
        renderInfo.shader->setUniformValue( "texture", 0 );
        renderInfo.shader->setUniformValue( renderInfo.shader->uniformLocation( "nowTexture" ), static_cast<GLint>( index ) );
        renderInfo.f->glDrawElements( GL_TRIANGLES, getVAOsize(), GL_UNSIGNED_INT, nullptr );
    }
    releaseVAO();
}
