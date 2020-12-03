#pragma once

#include <memory>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif
#include <GL/glu.h>
#include <GL/gl.h>

#include <QString>
#include <QVector>
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
#include "resources/resource_pointer.h"
#include "render_system/drawable.h"


class Mesh : public Drawable
{
public:
    Mesh();
    Mesh( ResourcePointer<Model> model );
    ~Mesh() = default;
    DrawBuffers getDrawBuffers() const override;

    void setMaterial( size_t index, Material newMaterial );
private:
    DrawBuffers m_submeshes;
};
