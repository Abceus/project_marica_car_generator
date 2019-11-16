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

    QVector<Vertex> vertices;
    vertices.reserve( model.vertices.size() );

    for( const auto& vertex: model.vertices )
    {
        vertices.append( { vertex.Y, vertex.Z, -vertex.X, vertex.U, vertex.V, vertex.MaterialIndex } );
    }

    VBO.bind();
    VBO.allocate( vertices.data(), sizeof( Vertex )*vertices.size() );

//    m_aabb = AABBBox( QVector3D( minX, maxY, maxZ ), QVector3D( maxX, minY, minZ ) );

    QVector<QVector<GLuint>> m_indices;
    for( auto& material: model.materials )
    {
        addTexture( material );
        m_indices.append(QVector<GLuint>());
    }

    for( size_t i = 0; i < model.indices.size(); i++ )
    {
        auto materialIndex = static_cast<size_t>( vertices[model.indices[i].vertexes[0]].MaterialIndex );
        for( auto j = 0; j < 3; j++ )
        {
            m_indices[materialIndex].append( model.indices[i].vertexes[j] );
        }
    }

    QVector<GLuint> resultIndices;
    size_t offset = 0;

    for( auto ind: m_indices )
    {
        m_buffersOffsets.append(offset);
        m_bufferSizes.append(ind.size());
        offset += ind.size();
        resultIndices += ind;
    }

    EBO.bind();
    EBO.allocate( resultIndices.data(), sizeof( GLuint )*resultIndices.size() );

    f->glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, sizeof( Vertex ), nullptr );
    f->glEnableVertexAttribArray( 0 );
    f->glVertexAttribPointer( 1, 2, GL_FLOAT, GL_FALSE, sizeof( Vertex ),
            reinterpret_cast<GLvoid*>( offsetof( Vertex, U ) ) );
    f->glEnableVertexAttribArray( 1 );

    VAO.release();

    VAOsize = model.VAOsize;

    m_model = model;

    for( size_t i = 0; i < m_buffersOffsets.size(); i++ )
    {
        QVector<Face> buffersFaces;
        for( size_t j = m_buffersOffsets[i]; j < m_buffersOffsets[i] + m_bufferSizes[i]; j++ )
        {
            Face f;
            f.indiceIndex = j;
            f.materialIndex = vertices[m_model.indices[j/3].vertexes[0]].MaterialIndex;
            for( size_t k = 0; k < 3; k++ )
            {
                f.points[k] = m_model.indices[j/3].vertexes[k];
            }
            buffersFaces.append( f );
        }
        m_faces.append( buffersFaces );
    }
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
        if( averageAlpha( image ) < 240.0f && !m_transparentBuffers.contains( index ) )
        {
            m_transparentBuffers.append( index );
        } else if( m_transparentBuffers.contains( index ) )
        {
            m_transparentBuffers.removeOne( index );
        }
    }
    else
    {
        //TODO: Load default image from texture manager
        image = ResourceManager::Instance().get<QString, QImage>( "./resources/textures/DefaultTexture.tga" );
        if( m_transparentBuffers.contains( index ) )
        {
            m_transparentBuffers.removeOne( index );
        }
    }
    textures[index]->destroy();
    this->textures[index] = QSharedPointer<QOpenGLTexture>( new QOpenGLTexture( image ) );
}

void Mesh::addTexture( QString filename )
{
    QImage image = ResourceManager::Instance().get<QString, QImage>( filename );
    if( image.isNull() )
    {
        //TODO: Load default image from texture manager
        image = ResourceManager::Instance().get<QString, QImage>( "./resources/textures/DefaultTexture.tga" );
    }
    this->textures.append( QSharedPointer<QOpenGLTexture>( new QOpenGLTexture( image ) ) );
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
    for( size_t i=0; i< m_bufferSizes.size(); i++ )
    {
        if( m_transparentBuffers.contains( i ) )
        {
            continue;
        }
        bindTexture( i );
        renderInfo.f->glDrawElements( GL_TRIANGLES, m_bufferSizes[i], GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>( m_buffersOffsets[i] * sizeof( GLuint ) ) );
    }
    releaseVAO();
}

void Mesh::subdraw(const RenderInfo &renderInfo, size_t materialIndex, size_t from, size_t size)
{
    if( size > 1 )
    {
        qDebug() << "hi";
    }
    bindVAO();
    bindTexture( materialIndex );
    renderInfo.f->glDrawElements( GL_TRIANGLES, size, GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>( from * sizeof( GLuint ) ) );
    releaseVAO();
}

QVector<Face> Mesh::getTransparentFaces() const
{
    QVector<Face> result;
    for( const auto& i: m_transparentBuffers )
    {
        result += m_faces[i];
    }
    return result;
}

QVector<Vertex> Mesh::getVertexes() const
{
    return QVector<Vertex>::fromStdVector( m_model.vertices );
}

AABBBox Mesh::getRootAABBB() const
{
    m_aabb;
}
