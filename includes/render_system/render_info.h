#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QOpenGLShaderProgram>

struct RenderInfo
{
    QOpenGLFunctions* f;
    QOpenGLExtraFunctions* ef;
    QOpenGLShaderProgram* shader;
};
