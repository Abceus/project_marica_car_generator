#pragma once

#include "drawable.h"
#include "resources/model.h"

#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>


class WireframeMesh : public Drawable
{
public:
    WireframeMesh( Model model );
    Model getModel();
    void draw( const RenderInfo& renderInfo ) override;
private:
    QOpenGLVertexArrayObject m_VAO;
    QOpenGLBuffer m_VBO, m_EBO;
    GLsizei m_VAOsize;
    Model m_model;
};
