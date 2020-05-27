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
    QVector<DrawBuffer> getDrawBuffer() const override;
private:
    QColor m_color;
};
