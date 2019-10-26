#pragma once

#include <memory>

#ifdef _WIN32
    #include <Windows.h>
#endif
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
#include "render_system/aabb.h"

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
    Model getModel();
    void draw( const RenderInfo& renderInfo ) override;
    void subdraw( const RenderInfo& renderInfo, size_t materialIndex, size_t from, size_t size ) override;
    QVector<Face> getTransparentFaces() const override;
    QVector<Vertex> getVertexes() const override;

    AABBBox getRootAABBB() const override;
private:
    QVector<size_t> m_buffersOffsets;
    QVector<size_t> m_bufferSizes;

    QVector<QVector<Face>> m_faces;

    QVector<size_t> m_transparentBuffers;

    QOpenGLVertexArrayObject VAO;
    QOpenGLBuffer VBO, EBO;
    QVector<QSharedPointer<QOpenGLTexture>> textures;
    GLsizei VAOsize;
    Model m_model;

    AABBBox m_aabb;

    float averageAlpha( QImage image );
};
