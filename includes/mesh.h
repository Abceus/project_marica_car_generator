#pragma once

#include <memory>

#include <GL/glu.h>
#include <GL/gl.h>

#include <QString>
#include <QVector>
#include <QVector4D>
#include <QQuaternion>
#include <QPicture>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

#include "unanimation.h"
#include "model.h"

class Mesh
{
public:
    Mesh();
    Mesh( const Model& model );
//    Mesh( Model&& model );
    ~Mesh();
    void bindVAO();
    void releaseVAO();
    void bindTexture( size_t index );
    void setTexture( QString filename, size_t index );
    void addTexture( QString filename );
    size_t getTexturesSize();
    GLsizei getVAOsize();
    size_t getTextureQueue( size_t index );
    void setTextureQueue( size_t index, float average );
    Model getModel();
private:
    std::unique_ptr<QOpenGLVertexArrayObject> VAO;
    std::unique_ptr<QOpenGLBuffer> VBO, EBO;
    std::vector<std::unique_ptr<QOpenGLTexture>> textures;
    std::vector<std::pair<size_t, float>> textureQueue;
    GLsizei VAOsize;
    void sortTextures();
    float averageAlpha( QImage image );
    Model model;
};
