#include "mesh.h"

#include <QFile>
#include <memory>


Mesh::Mesh()
        : VAOsize(0u)
{
}

Mesh::Mesh(const Model& model)
{
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();
    if(!f)
        return;

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
    this->VBO->allocate(model.vertices.data(), sizeof(Vertex)*model.vertices.size());

    this->EBO->bind();
    this->EBO->allocate(model.indices.data(), sizeof(Indice)*model.indices.size());

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    f->glEnableVertexAttribArray(0);
    f->glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, U)));
    f->glEnableVertexAttribArray(1);
    f->glVertexAttribPointer(2, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, MaterialIndex)));
    f->glEnableVertexAttribArray(2);

    this->VAO->release();

    this->VAOsize = model.VAOsize;

    for( auto& material: model.materials )
    {
        addTexture( material );
    }
}

Mesh::~Mesh()
{
    VAO->destroy();
    VBO->destroy();
    EBO->destroy();
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

void Mesh::addTexture(QString filename)
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
        image = QImage("./resources/textures/skin1.png");
        this->textureQueue.emplace_back(this->textures.size(), this->averageAlpha(image));
        this->textures.emplace_back(std::make_unique<QOpenGLTexture>(image));
    }
    this->sortTextures();
}

void Mesh::sortTextures()
{
    std::sort(this->textureQueue.begin(), this->textureQueue.end(),
              [](const std::pair<int, float>& lhs,
                 const std::pair<int, float>& rhs){ return lhs.second > rhs.second; });
}

size_t Mesh::getTextureQueue( size_t index )
{
    return this->textureQueue.at(index).first;
}

float Mesh::averageAlpha(QImage image)
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

void Mesh::setTextureQueue( size_t index, float average )
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

void Mesh::bindVAO()
{
    VAO->bind();
}

void Mesh::releaseVAO()
{
    VAO->release();
}

void Mesh::bindTexture( size_t index )
{
    this->textures.at(index)->bind();
}
