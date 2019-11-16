#pragma once

#include "render_system/drawable.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include "vector3d.h"
#include <QColor>

class Box : public Drawable
{
public:
    Box( Vector3D size, QColor color = QColor( 0, 0, 0, 255 ) );
    void draw( const RenderInfo& renderInfo ) override;
private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO, m_EBO;
    QColor m_color;
};
