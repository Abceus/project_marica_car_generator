#pragma once

#include "drawable.h"
#include "resources/wireframe_model.h"
#include "resources/resource_pointer.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QColor>

class WireframeMesh : public Drawable
{
public:
    WireframeMesh() = default;
    WireframeMesh( const WireframeModel& model, QColor color = QColor( 0, 0, 0, 255 ) );
    DrawBuffers getDrawBuffers() const override;
private:
    DrawBuffers m_submeshes;
    QColor m_color;
};
