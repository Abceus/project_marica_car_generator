#include "model.h"

#include <QFile>
#include <memory>
#include <includes/model.h>


Model::Model()
    : VAOsize(0u)
{
}

Model::Model(const QString& filename, QOpenGLFunctions *f, QOpenGLExtraFunctions* /*ef*/)
{
    VChunkHeader GeneralHeader{};
    VChunkHeader PointsHeader{};
    std::vector<VPoint> PointsData;
    VChunkHeader WedgesHeader{};
    std::vector<VVertex> WedgesData;
    VChunkHeader FacesHeader{};
    std::vector<VTriangle> FacesData;
    VChunkHeader MaterialsHeader{};
    std::vector<VMaterial> MaterialsData;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return;

    file.read(reinterpret_cast<char*>(&GeneralHeader), sizeof(VChunkHeader));
    qDebug() << GeneralHeader.ChunkID << " " << GeneralHeader.DataCount << " " << GeneralHeader.DataSize << " " << GeneralHeader.TypeFlags;
    file.read(reinterpret_cast<char*>(&PointsHeader), sizeof(VChunkHeader));
    qDebug() << PointsHeader.ChunkID << " " << PointsHeader.DataCount << " " << PointsHeader.DataSize << " " << PointsHeader.TypeFlags;
    PointsData.reserve(static_cast<size_t>(PointsHeader.DataCount));
    file.read( reinterpret_cast<char*>(PointsData.data()), PointsHeader.DataSize * PointsHeader.DataCount );

    file.read(reinterpret_cast<char*>(&WedgesHeader), sizeof(VChunkHeader));
    qDebug() << WedgesHeader.ChunkID << " " << WedgesHeader.DataCount << " " << WedgesHeader.DataSize << " " << WedgesHeader.TypeFlags;
    WedgesData.reserve(static_cast<size_t>(WedgesHeader.DataCount));
    file.read(reinterpret_cast<char*>(WedgesData.data()), WedgesHeader.DataSize * WedgesHeader.DataCount);

    file.read(reinterpret_cast<char*>(&FacesHeader), sizeof(VChunkHeader));
    qDebug() << FacesHeader.ChunkID << " " << FacesHeader.DataCount << " " << FacesHeader.DataSize << " " << FacesHeader.TypeFlags;
    FacesData.reserve(static_cast<size_t>(FacesHeader.DataCount));
    file.read(reinterpret_cast<char*>(FacesData.data()), FacesHeader.DataSize * FacesHeader.DataCount);

    file.read(reinterpret_cast<char*>(&MaterialsHeader), sizeof(VChunkHeader));
    qDebug() << MaterialsHeader.ChunkID << " " << MaterialsHeader.DataCount << " " << MaterialsHeader.DataSize << " " << MaterialsHeader.TypeFlags;
    MaterialsData.reserve(static_cast<size_t>(MaterialsHeader.DataCount));
    file.read(reinterpret_cast<char*>(MaterialsData.data()), MaterialsHeader.DataSize * MaterialsHeader.DataCount);

    file.close();

    std::vector<GLfloat> vertices(WedgesHeader.DataCount*6u);

    for(size_t i=0; i<WedgesHeader.DataCount; i++)
    {
        vertices[i*6u] = PointsData[WedgesData[i].PointIndex].X;
        vertices[i*6u+1u] = PointsData[WedgesData[i].PointIndex].Y;
        vertices[i*6u+2u] = PointsData[WedgesData[i].PointIndex].Z;
        vertices[i*6u+3u] = WedgesData[i].U;
        vertices[i*6u+4u] = WedgesData[i].V;
        vertices[i*6u+5u] = GLfloat(WedgesData[i].MatIndex);
    }

    std::vector<GLuint> indices(FacesHeader.DataCount*3u);

    for(size_t i=0; i<FacesHeader.DataCount; i++)
    {
        for(size_t j=0; j<3; j++)
        {
            indices[i*3u+j] = FacesData[i].WedgeIndex[j];
        }
    }

    this->VAO = std::make_unique<QOpenGLVertexArrayObject>();
    this->VAO->create();
    this->VBO = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::VertexBuffer);
    this->VBO->create();
    this->VBO->setUsagePattern(QOpenGLBuffer::StaticDraw);
    this->EBO = std::make_unique<QOpenGLBuffer>(QOpenGLBuffer::IndexBuffer);
    this->EBO->create();
    this->EBO->setUsagePattern(QOpenGLBuffer::StaticDraw);

    this->VAO->bind();

    this->VBO->bind();
    this->VBO->allocate(vertices.data(), sizeof(VVertex)*WedgesHeader.DataCount*6u);

    this->EBO->bind();
    this->EBO->allocate(indices.data(), sizeof(VTriangle)*FacesHeader.DataCount*3u);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(5 * sizeof(GLfloat)));
    f->glEnableVertexAttribArray(2);

    this->VAO->release();

    this->VAOsize = FacesHeader.DataCount*3u;

    for(size_t i=0; i<MaterialsHeader.DataCount; i++)
    {
        this->addTexture(QString("./") + QString(MaterialsData[i].MaterialName));
    }
}

Model::~Model()
{
    VAO->destroy();
    VBO->destroy();
    EBO->destroy();
}

size_t Model::getVAOsize()
{
    return this->VAOsize;
}

size_t Model::getTexturesSize()
{
    return this->textures.size();
}

void Model::setTexture( QString filename, size_t index )
{
    QImage image = QImage(filename);
    if(!image.isNull())
    {
        this->setTextureQueue(index, this->averageAlpha(image));
        this->textures[index] = std::make_unique<QOpenGLTexture>(image);
    }
    else
    {
        //TODO: Load default image from texture manager
        image = QImage("./resources/textures/test.jpg");
        this->setTextureQueue(index, this->averageAlpha(image));
        this->textures[index] = std::make_unique<QOpenGLTexture>(image);
    }
    this->sortTextures();
}

void Model::addTexture(QString filename)
{
    QImage image = QImage(filename);
    if(!image.isNull())
    {
        this->textureQueue.emplace_back(this->textures.size(), this->averageAlpha(image));
        this->textures.emplace_back(std::make_unique<QOpenGLTexture>(image));
    }
    else
    {
        //TODO: Load default image from texture manager
        image = QImage("./resources/textures/test.jpg");
        this->textureQueue.emplace_back(this->textures.size(), this->averageAlpha(image));
        this->textures.emplace_back(std::make_unique<QOpenGLTexture>(image));
    }
    this->sortTextures();
}

void Model::sortTextures()
{
    std::sort(this->textureQueue.begin(), this->textureQueue.end(),
              [](const std::pair<int, float>& lhs,
              const std::pair<int, float>& rhs){ return lhs.second > rhs.second; });
}

size_t Model::getTextureQueue( size_t index )
{
    return this->textureQueue.at(index).first;
}

float Model::averageAlpha(QImage image)
{
    float sum = 0;
    for(int i=0; i<image.height(); i++)
    {
        for(int j=0; j<image.width(); j++)
        {
            sum += image.pixelColor(j, i).alpha();
        }
    }
    return sum/(image.height()*image.width());
}

void Model::setTextureQueue( size_t index, float average )
{
    for(auto& texture: textureQueue)
    {
        if(texture.first == index)
        {
            texture.second = average;
            return;
        }
    }
}

void Model::bindVAO()
{
    VAO->bind();
}

void Model::releaseVAO()
{
    VAO->release();
}

void Model::bindTexture( size_t index )
{
    this->textures.at(index)->bind();
}
