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
    void init();
    void draw( QSharedPointer<Scene> scene );

//    template<typename T, typename... Args>
//    QSharedPointer<T> makeDrawable( Args... args );

    QSharedPointer<Camera> getMainCamera() const;
    void setMainCamera( QSharedPointer<Camera> camera );
private:
    void makeCurrent();
    void done();
    QOpenGLContext* m_context;
    QSurface* m_surface;

    Batching m_bathing;

    QSharedPointer<Camera> m_mainCamera;
};

//template<typename T, typename... Args>
//QSharedPointer<T> Renderer::makeDrawable( Args... args )
//{
//    makeCurrent();
//    QSharedPointer<T> result( new T( args... ) );
//    done();
//    return result;
//}
