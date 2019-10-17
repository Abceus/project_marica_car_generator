#include <algorithm>
#include <QtMath>

#include "mainopenglwidget.h"

MainOpenglWidget::MainOpenglWidget( QWidget *parent )
    : QOpenGLWidget( parent )
    , m_scene(new Scene)
{
    setMouseTracking( true );
    leftPressed = false;
    rightPressed = false;
    setCursor( Qt::CrossCursor );
}

void MainOpenglWidget::initializeGL()
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor( 0.25f, 0.25f, 0.25f, 1.0f );

    // Enable depth buffer
    f->glEnable( GL_DEPTH_TEST );

    f->glEnable( GL_BLEND );
    f->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    m_renderer = Renderer( context(), context()->surface() );

    auto vertexShader = m_renderer.loadShader( "resources/shaders/defaultvertexshader.vert", QOpenGLShader::Vertex );

    if( !vertexShader )
    {
        qDebug() << "The vertex shader wasn't compiled";
    }

    auto fragmentShader = m_renderer.loadShader( "resources/shaders/defaultfragmentshader.frag", QOpenGLShader::Fragment );

    if( !fragmentShader )
    {
        qDebug() << "The fragment shader wasn't compiled";
    }

    auto program = m_renderer.getShaderProgram( vertexShader, fragmentShader );

    m_scene->init( program );

    doneCurrent();
}

void MainOpenglWidget::paintGL()
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear( GL_COLOR_BUFFER_BIT );

    m_renderer.draw( m_scene );

    update();
}

void MainOpenglWidget::resizeGL( int w, int h )
{
    m_scene->resizeScreen( w, h );
}

void MainOpenglWidget::setBodyObject( QSharedPointer<Object> object )
{
    m_body = object;
}

Object* MainOpenglWidget::getBodyObject() const
{
    return m_body.get();
}

void MainOpenglWidget::setBodyTexture( const QString &filename, size_t index )
{
    m_body->getDraweable()->setTexture( filename, index );
}

QSharedPointer<Scene> MainOpenglWidget::getScene()
{
    return m_scene;
}

Renderer &MainOpenglWidget::getRenderer()
{
    return m_renderer;
}

void MainOpenglWidget::mousePressEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton )
    {
        leftClickPos = event->pos();
        leftPressed = true;
        setCursor(Qt::BlankCursor);
    }
    else if( event->button() == Qt::RightButton )
    {
        rightClickPos = event->pos();
        rightPressed = true;
        setCursor(Qt::BlankCursor);
    }
}

void MainOpenglWidget::mouseReleaseEvent( QMouseEvent *event )
{
    if( event->button() == Qt::LeftButton )
    {
        leftPressed = false;
        if( !rightPressed )
        {
            setCursor(Qt::CrossCursor);
        }
    }
    else if( event->button() == Qt::RightButton )
    {
        rightPressed = false;
        if( !leftPressed )
        {
            setCursor(Qt::CrossCursor);
        }
    }
}

void MainOpenglWidget::mouseMoveEvent( QMouseEvent *event )
{
    if( leftPressed && rightPressed )
    {
        QVector3D currentCameraLocation = m_scene->getCameraLocation();
        currentCameraLocation.setY( currentCameraLocation.y() - ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        m_scene->setCameraLocation( currentCameraLocation );
    }
    else if( leftPressed )
    {
        QVector3D camera_rotation_now = m_scene->getCameraRotation();
        camera_rotation_now.setY( camera_rotation_now.y() - ( leftClickPos.x() - event->x() ) / 5 );
        auto cameraLocationNow = m_scene->getCameraLocation();
        cameraLocationNow.setZ( cameraLocationNow.z() + qCos( qDegreesToRadians( camera_rotation_now.y() ) ) * ( leftClickPos.y() - event->y() ) * 10 );
        cameraLocationNow.setX( cameraLocationNow.x() - qSin( qDegreesToRadians( camera_rotation_now.y() ) ) * ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        m_scene->setCameraRotation( camera_rotation_now );
        m_scene->setCameraLocation( cameraLocationNow );
    }
    else if( rightPressed )
    {
        QVector3D camera_rotation_now = m_scene->getCameraRotation();
        camera_rotation_now.setY( camera_rotation_now.y() - ( rightClickPos.x() - event->x() ) / 5 );
        camera_rotation_now.setX( camera_rotation_now.x() - ( rightClickPos.y() - event->y() ) / 5 );
        auto c = cursor();
        c.setPos( mapToGlobal( rightClickPos ) );
        setCursor(c);
        m_scene->setCameraRotation( camera_rotation_now );
    }
}
