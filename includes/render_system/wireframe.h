#pragma once

#include "drawable.h"
#include "resources/model.h"
#include "resources/resource_pointer.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QColor>

class WireframeMesh : public Drawable
{
public:
    WireframeMesh( ResourcePointer<Model> model, QColor color = QColor( 0, 0, 0, 255 ) );
    QVector<DrawBuffer> getDrawBuffer() const override;
private:
    QVector<DrawBuffer> m_submeshes;
    QColor m_color;
};
