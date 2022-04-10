#include "render_system/renderer.h"
#include "utils/scope_guard.h"
#include <qopenglcontext.h>

std::stack<Renderer*> Renderer::rendererStack;

Renderer::Renderer(QOpenGLContext *context, QSurface *surface)
    : m_context( context )
    , m_surface( surface )
{

}

void Renderer::draw(QSharedPointer<Scene> scene)
{
    pushRenderer(this);
    gc.collect();
    scene->draw( m_context->functions(), m_context->extraFunctions() );
    popRenderer();
}

QSharedPointer<QOpenGLShader> Renderer::loadShader(QString path, QOpenGLShader::ShaderTypeBit type)
{
    auto found = m_shaders.find( path );

    if( found == m_shaders.end() )
    {
        makeCurrent();
        QSharedPointer<QOpenGLShader> shader( new QOpenGLShader( type ) );

        bool success = shader->compileSourceFile( path );
        if( !success )
        {
            qDebug() << "The vertex shader wasn't compiled";
            return nullptr;
        }
        m_shaders.insert( path, shader );
        return shader;
    }

    return *found;
}

QSharedPointer<QOpenGLShaderProgram> Renderer::getShaderProgram(QSharedPointer<QOpenGLShader> vertexShader, QSharedPointer<QOpenGLShader> fragmentShader)
{
    makeCurrent();
    auto found = m_programs.find( QPair<QSharedPointer<QOpenGLShader>, QSharedPointer<QOpenGLShader>>( vertexShader, fragmentShader ) );
    if( found == m_programs.end() )
    {
        QSharedPointer<QOpenGLShaderProgram> shaderProgram( new QOpenGLShaderProgram );
        shaderProgram->addShader( vertexShader.get() );
        shaderProgram->addShader( fragmentShader.get() );
        if( !shaderProgram->link() )
        {
            qDebug() << "The shader program wasn't linked";
            return nullptr;
        }
        m_programs.insert( QPair<QSharedPointer<QOpenGLShader>, QSharedPointer<QOpenGLShader>>( vertexShader, fragmentShader ), shaderProgram );
        return shaderProgram;
    }
    return *found;
}

void Renderer::makeCurrent()
{
    m_context->makeCurrent( m_surface );
}

void Renderer::done()
{
    m_context->doneCurrent();
}

ScopeGuard Renderer::pushContextScoped()
{
    return ScopeGuard([this](){ popRenderer(); });
}

void Renderer::pushRenderer(Renderer* newRenderer) 
{
    rendererStack.push(newRenderer);
    newRenderer->makeCurrent();
}

void Renderer::popRenderer() 
{
    rendererStack.top()->done();
    rendererStack.pop();
    if(!rendererStack.empty()) 
    {
        rendererStack.top()->makeCurrent();
    }
}

Renderer* Renderer::getCurrentRenderer() 
{
    return rendererStack.top();
}