#pragma once

#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <QMatrix4x4>
#include <QOpenGLShaderProgram>

#include "render_system/material.h"

enum ElementsType
{
    PT_Vertex,
    PT_Line
};

enum DrawType
{
    DT_Mesh,
    DT_Overlay
};

struct DrawBuffer
{
    struct Vertex
    {
        QVector3D coords;
        QVector2D textureCoords;
    };

    QVector<Vertex> vertices;
    QVector<unsigned> indices;
    ElementsType elementType;
    DrawType drawType;
    Material material;

    void clear();
};

struct DrawBuffers : public QVector<DrawBuffer>{};

DrawBuffers operator+(DrawBuffers lhs, const DrawBuffers& rhs);
DrawBuffers operator*(DrawBuffers lhs, const QMatrix4x4& rhs);

inline bool operator==(const DrawBuffer& lhs, const DrawBuffer& rhs)
{
    return lhs.material.texture == rhs.material.texture && lhs.material.blendType == rhs.material.blendType && lhs.material.color == rhs.material.color &&
            lhs.elementType == rhs.elementType && lhs.drawType == rhs.drawType;
}

inline bool operator!=(const DrawBuffer& lhs, const DrawBuffer& rhs){ return !(lhs == rhs); }

DrawBuffer operator+(DrawBuffer lhs, const DrawBuffer& rhs);
DrawBuffer operator*(DrawBuffer lhs, const QMatrix4x4& rhs);

class Batching
{
public:
    void init();
    void addElements( const DrawBuffer& drawbuffer );
    void clear();
    void draw();
private:
    DrawBuffers m_meshes;
    DrawBuffers m_transparentMeshes;
    DrawBuffer m_overlays;
    QSharedPointer<QOpenGLShaderProgram> m_textureShader;
    QSharedPointer<QOpenGLShaderProgram> m_lineShader;
};
