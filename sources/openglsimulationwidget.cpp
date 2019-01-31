#include "openglsimulationwidget.h"

OpenglSimulationWidget::OpenglSimulationWidget( QWidget *parent )
        : QOpenGLWidget( parent )
{
}

void OpenglSimulationWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );

    // Enable depth buffer
    f->glEnable( GL_DEPTH_TEST );

    f->glEnable( GL_BLEND );
    f->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    std::unique_ptr<QOpenGLShader> vertexShader( new QOpenGLShader( QOpenGLShader::Vertex ) );

    bool success = vertexShader->compileSourceFile( "resources/shaders/defaultvertexshader.vert" );

    if( !success )
    {
        qDebug() << "The vertex shader wasn't compiled";
    }

    std::unique_ptr<QOpenGLShader> fragmentShader( new QOpenGLShader( QOpenGLShader::Fragment ) );
    success = fragmentShader->compileSourceFile( "resources/shaders/defaultfragmentshader.frag" );

    if( !success )
    {
        qDebug() << "The fragment shader wasn't compiled";
    }

    ShaderProgram = std::make_unique<QOpenGLShaderProgram>();
    ShaderProgram->addShader( vertexShader.get() );
    ShaderProgram->addShader( fragmentShader.get() );
    if( !ShaderProgram->link() )
    {
        qDebug() << "The shader program wasn't linked";
    }

    ShaderProgram->bind();

    makeCurrent();
}

void OpenglSimulationWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear( GL_COLOR_BUFFER_BIT );

    ShaderProgram->bind();

    QMatrix4x4 view;

    ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "view" ), view );
    ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "projection" ), projection );

    QMatrix4x4 model;
    ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "model" ), model );

    if( keys[Qt::Key_Up] )
    {
        qDebug() << "Up";
    }
    if( keys[Qt::Key_Down] )
    {
        qDebug() << "Down";
    }
    if( keys[Qt::Key_Left] )
    {
        qDebug() << "Left";
    }
    if( keys[Qt::Key_Right] )
    {
        qDebug() << "Right";
    }
    update();
}

void OpenglSimulationWidget::resizeGL( int w, int h )
{
    // Calculate aspect ratio
    const float aspect = static_cast<float>( w ) / static_cast<float>( h ? h : 1 );
    const float zNear = 1.f, zFar = 10000.f, fov = 90.f;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective( fov, aspect, zNear, zFar );
}

void OpenglSimulationWidget::keyPressEvent( QKeyEvent* event )
{
    keys[event->key()] = true;
}

void OpenglSimulationWidget::keyReleaseEvent( QKeyEvent* event )
{
    keys[event->key()] = false;
}
