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

//QImage loadTga(const char* filePath, bool &success)
//{
//    QImage img;
//    if (!img.load(filePath))
//    {

//        // open the file
//        std::fstream fsPicture(filePath, std::ios::in | std::ios::binary);

//        if (!fsPicture.is_open())
//        {
//            img = QImage(1, 1, QImage::Format_RGB32);
//            img.fill(Qt::red);
//            success = false;
//            return img;
//        }

//        // some variables
//        std::vector<std::uint8_t>* vui8Pixels;
//        std::uint32_t ui32BpP;
//        std::uint32_t ui32Width;
//        std::uint32_t ui32Height;

//        // read in the header
//        std::uint8_t ui8x18Header[19] = { 0 };
//        fsPicture.read(reinterpret_cast<char*>(&ui8x18Header), sizeof(ui8x18Header) - 1);

//        //get variables
//        vui8Pixels = new std::vector<std::uint8_t>;
//        bool bCompressed;
//        std::uint32_t ui32IDLength;
//        std::uint32_t ui32PicType;
//        std::uint32_t ui32PaletteLength;
//        std::uint32_t ui32Size;

//        // extract all information from header
//        ui32IDLength = ui8x18Header[0];
//        ui32PicType = ui8x18Header[2];
//        ui32PaletteLength = ui8x18Header[6] * 0x100 + ui8x18Header[5];
//        ui32Width = ui8x18Header[13] * 0x100 + ui8x18Header[12];
//        ui32Height = ui8x18Header[15] * 0x100 + ui8x18Header[14];
//        ui32BpP = ui8x18Header[16];

//        // calculate some more information
//        ui32Size = ui32Width * ui32Height * ui32BpP / 8;
//        bCompressed = ui32PicType == 9 || ui32PicType == 10;
//        vui8Pixels->resize(ui32Size);

//        // jump to the data block
//        fsPicture.seekg(ui32IDLength + ui32PaletteLength, std::ios_base::cur);

//        if (ui32PicType == 2 && (ui32BpP == 24 || ui32BpP == 32))
//        {
//            fsPicture.read(reinterpret_cast<char*>(vui8Pixels->data()), ui32Size);
//        }
//        // else if compressed 24 or 32 bit
//        else if (ui32PicType == 10 && (ui32BpP == 24 || ui32BpP == 32))	// compressed
//        {
//            std::uint8_t tempChunkHeader;
//            std::uint8_t tempData[5];
//            unsigned int tempByteIndex = 0;

//            do {
//                fsPicture.read(reinterpret_cast<char*>(&tempChunkHeader), sizeof(tempChunkHeader));

//                if (tempChunkHeader >> 7)	// repeat count
//                {
//                    // just use the first 7 bits
//                    tempChunkHeader = (uint8_t(tempChunkHeader << 1) >> 1);

//                    fsPicture.read(reinterpret_cast<char*>(&tempData), ui32BpP / 8);

//                    for (int i = 0; i <= tempChunkHeader; i++)
//                    {
//                        vui8Pixels->at(tempByteIndex++) = tempData[0];
//                        vui8Pixels->at(tempByteIndex++) = tempData[1];
//                        vui8Pixels->at(tempByteIndex++) = tempData[2];
//                        if (ui32BpP == 32) vui8Pixels->at(tempByteIndex++) = tempData[3];
//                    }
//                }
//                else						// data count
//                {
//                    // just use the first 7 bits
//                    tempChunkHeader = (uint8_t(tempChunkHeader << 1) >> 1);

//                    for (int i = 0; i <= tempChunkHeader; i++)
//                    {
//                        fsPicture.read(reinterpret_cast<char*>(&tempData), ui32BpP / 8);

//                        vui8Pixels->at(tempByteIndex++) = tempData[0];
//                        vui8Pixels->at(tempByteIndex++) = tempData[1];
//                        vui8Pixels->at(tempByteIndex++) = tempData[2];
//                        if (ui32BpP == 32) vui8Pixels->at(tempByteIndex++) = tempData[3];
//                    }
//                }
//            } while (tempByteIndex < ui32Size);
//        }
//        // not useable format
//        else
//        {
//            fsPicture.close();
//            img = QImage(1, 1, QImage::Format_RGB32);
//            img.fill(Qt::red);
//            success = false;
//            return img;
//        }

//        fsPicture.close();

//        img = QImage(ui32Width, ui32Height, QImage::Format_RGB888);

//        int pixelSize = ui32BpP == 32 ? 4 : 3;
//        //TODO: write direct into img
//        for (unsigned int x = 0; x < ui32Width; x++)
//        {
//            for (unsigned int y = 0; y < ui32Height; y++)
//            {
//                int valr = vui8Pixels->at(y * ui32Width * pixelSize + x * pixelSize + 2);
//                int valg = vui8Pixels->at(y * ui32Width * pixelSize + x * pixelSize + 1);
//                int valb = vui8Pixels->at(y * ui32Width * pixelSize + x * pixelSize);

//                QColor value(valr, valg, valb);
//                img.setPixelColor(x, y, value);
//            }
//        }

//        img = img.mirrored();

//    }
//    success = true;
//    return img;
//}

void Mesh::addTexture( QString filename )
{
    QImage image = ResourceManager::Instance().get<QString, QImage>( filename );
    if( image.isNull() )
    {
        //TODO: Load default image from texture manager
        image = ResourceManager::Instance().get<QString, QImage>( "./resources/textures/DefaultTexture.tga" );
        //bool success;
        //image = loadTga( "./resources/textures/DefaultTexture.tga", success );
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
