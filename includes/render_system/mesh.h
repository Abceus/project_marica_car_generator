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
#include <QSharedPointer>

#include "resources/unanimation.h"
#include "resources/model.h"
#include "render_system/drawable.h"
#include "render_system/render_info.h"

class Mesh : public Drawable
{
public:
    Mesh();
    Mesh( const Model& model );
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
    void draw( const RenderInfo& renderInfo ) override;
private:
    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO, EBO;
    QVector<QSharedPointer<QOpenGLTexture>> textures;
    std::vector<std::pair<size_t, float>> textureQueue;
    GLsizei VAOsize;
    void sortTextures();
    float averageAlpha( QImage image );
    Model m_model;
};
