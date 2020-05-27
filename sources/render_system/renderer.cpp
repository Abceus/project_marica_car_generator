#include "render_system/renderer.h"

Renderer::Renderer(QOpenGLContext *context, QSurface *surface)
    : m_context( context )
    , m_surface( surface )
{
}

void Renderer::init()
{
    makeCurrent();
    m_bathing.init();
    done();
}

void Renderer::draw(QSharedPointer<Scene> scene)
{
    if( m_mainCamera )
    {
        makeCurrent();
        scene->draw( m_bathing, m_mainCamera );
        m_bathing.draw();
        m_bathing.clear();
        done();
    }
}

void Renderer::makeCurrent()
{
    m_context->makeCurrent( m_surface );
}

void Renderer::done()
{
    m_context->doneCurrent();
}

QSharedPointer<Camera> Renderer::getMainCamera() const
{
    return m_mainCamera;
}

void Renderer::setMainCamera( QSharedPointer<Camera> camera )
{
    m_mainCamera = camera;
}
