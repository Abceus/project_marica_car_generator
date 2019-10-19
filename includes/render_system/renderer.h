#pragma once

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QSurface>
#include <QSharedPointer>
#include <QMap>
#include <QString>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QPair>

#include "drawable.h"
#include "scene.h"

class Renderer
{
public:
    Renderer( QOpenGLContext* context = nullptr, QSurface* surface = nullptr );
    void draw( QSharedPointer<Scene> scene );

    template<typename T, typename... Args>
    QSharedPointer<T> makeDrawable( Args... args );

    QSharedPointer<QOpenGLShader> loadShader( QString path, QOpenGLShader::ShaderTypeBit type );
    QSharedPointer<QOpenGLShaderProgram> getShaderProgram( QSharedPointer<QOpenGLShader> vertexShader, QSharedPointer<QOpenGLShader> fragmentShader );
private:
    void makeCurrent();
    void done();
    QOpenGLContext* m_context;
    QSurface* m_surface;

    // TODO: rework
    QMap<QString, QSharedPointer<QOpenGLShader>> m_shaders;
    QMap<QPair<QSharedPointer<QOpenGLShader>, QSharedPointer<QOpenGLShader>>, QSharedPointer<QOpenGLShaderProgram>> m_programs;
};

template<typename T, typename... Args>
QSharedPointer<T> Renderer::makeDrawable( Args... args )
{
    makeCurrent();
    QSharedPointer<T> result( new T( args... ) );
    done();
    return result;
}
