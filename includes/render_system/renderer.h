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
#include "garbage_collector.h"
#include <stack>
#include "utils/scope_guard.h"

class Renderer
{
public:
    Renderer( QOpenGLContext* context = nullptr, QSurface* surface = nullptr );
    void draw( QSharedPointer<Scene> scene );

    template<typename T, typename... Args>
    QSharedPointer<T> makeDrawable( Args&&... args );

    QSharedPointer<QOpenGLShader> loadShader( QString path, QOpenGLShader::ShaderTypeBit type );
    QSharedPointer<QOpenGLShaderProgram> getShaderProgram( QSharedPointer<QOpenGLShader> vertexShader, QSharedPointer<QOpenGLShader> fragmentShader );

    ScopeGuard pushContextScoped();

    static void pushRenderer(Renderer* newRenderer);
    static void popRenderer();
    static Renderer* getCurrentRenderer();
private:
    void makeCurrent();
    void done();
    QOpenGLContext* m_context;
    QSurface* m_surface;
    GarbageCollector gc;

    // TODO: rework
    QMap<QString, QSharedPointer<QOpenGLShader>> m_shaders;
    QMap<QPair<QSharedPointer<QOpenGLShader>, QSharedPointer<QOpenGLShader>>, QSharedPointer<QOpenGLShaderProgram>> m_programs;

    static std::stack<Renderer*> rendererStack;
};

template<typename T, typename... Args>
QSharedPointer<T> Renderer::makeDrawable( Args&&... args )
{
    pushRenderer(this);
    QSharedPointer<T> result( new T( std::forward<Args>(args)... ),
        [this](T* obj)
        {
            gc.addResource(obj); 
        } 
    );
    return result;
}
