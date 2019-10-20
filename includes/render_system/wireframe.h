#pragma once

#include "drawable.h"
#include "resources/model.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QColor>

class WireframeMesh : public Drawable
{
public:
    WireframeMesh( Model model, QColor color = QColor( 0, 0, 0, 255 ) );
    Model getModel();
    void draw( const RenderInfo& renderInfo ) override;
private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO, m_EBO;
    GLsizei m_VAOsize;
    Model m_model;
    QColor m_color;
};
