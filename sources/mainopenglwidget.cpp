#include <algorithm>
#include <QtMath>

#include "mainopenglwidget.h"
#include "render_system/wireframe.h"
#include "render_system/box.h"

MainOpenglWidget::MainOpenglWidget( QWidget *parent )
    : QOpenGLWidget( parent )
    , m_scene(new Scene)
{
    setMouseTracking( true );
    leftPressed = false;
    rightPressed = false;
    setCursor( Qt::CrossCursor );

    auto node = m_scene->addNode( QSharedPointer<SceneNode>( new SceneNode ) );
    m_body = QSharedPointer<Object>( new Object( nullptr, node ) );
    m_collisionBody = QSharedPointer<Object>( new Object( nullptr, m_body->getNode()->addChild( QSharedPointer<SceneNode>( new SceneNode ) ) ) );

    m_leftSteerWheel = QSharedPointer<Object>( new Object( nullptr, m_body->getNode()->addChild( QSharedPointer<SceneNode>( new SceneNode ) ) ) );
    m_rightSteerWheel = QSharedPointer<Object>( new Object( nullptr, m_body->getNode()->addChild( QSharedPointer<SceneNode>( new SceneNode ) ) ) );
    m_leftEngWheel = QSharedPointer<Object>( new Object( nullptr, m_body->getNode()->addChild( QSharedPointer<SceneNode>( new SceneNode ) ) ) );
    m_rightEngWheel = QSharedPointer<Object>( new Object( nullptr, m_body->getNode()->addChild( QSharedPointer<SceneNode>( new SceneNode ) ) ) );
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

    auto vertexShader = m_renderer.loadShader( "resources/shaders/colorvertexshader.vert", QOpenGLShader::Vertex );

    if( !vertexShader )
    {
        qDebug() << "The vertex shader wasn't compiled";
    }

    auto fragmentShader = m_renderer.loadShader( "resources/shaders/colorfragmentshader.frag", QOpenGLShader::Fragment );

    if( !fragmentShader )
    {
        qDebug() << "The fragment shader wasn't compiled";
    }

    auto program = m_renderer.getShaderProgram( vertexShader, fragmentShader );

    m_scene->init( program );

    auto meshVertexShader = m_renderer.loadShader( "resources/shaders/texturevertexshader.vert", QOpenGLShader::Vertex );

    if( !meshVertexShader )
    {
        qDebug() << "The mesh vertex shader wasn't compiled";
    }

    auto meshFragmentShader = m_renderer.loadShader( "resources/shaders/texturefragmentshader.frag", QOpenGLShader::Fragment );

    if( !meshFragmentShader )
    {
        qDebug() << "The mesh fragment shader wasn't compiled";
    }

    auto meshProgram = m_renderer.getShaderProgram( meshVertexShader, meshFragmentShader );

    m_body->getNode()->setShaderProgram( meshProgram );

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

void MainOpenglWidget::setBodyMesh(QSharedPointer<Mesh> bodyMesh)
{
    m_body->setDrawable( bodyMesh );
}

 QSharedPointer<Object> MainOpenglWidget::getBodyObject() const
{
     return m_body;
 }

 QSharedPointer<Object> MainOpenglWidget::getCollisionBodyObject() const
 {
     return m_collisionBody;
 }

void MainOpenglWidget::setBodyTexture( const QString &filename, size_t index )
{
    m_body->getDrawable().staticCast<Mesh>()->setTexture( filename, index );
}

Model MainOpenglWidget::getBodyCollisionModel() const
{
    return m_collisionBody->getDrawable().staticCast<WireframeMesh>()->getModel();
}

Model MainOpenglWidget::getWheelCollisionModel() const
{
    return m_leftEngWheel->getDrawable().staticCast<WireframeMesh>()->getModel();
}

QSharedPointer<Object> MainOpenglWidget::getLeftSteerWheel() const
{
    return m_leftSteerWheel;
}

QSharedPointer<Object> MainOpenglWidget::getRightSteerWheel() const
{
    return m_rightSteerWheel;
}

QSharedPointer<Object> MainOpenglWidget::getLeftEngWheel() const
{
    return m_leftEngWheel;
}

QSharedPointer<Object> MainOpenglWidget::getRightEngWheel() const
{
    return m_rightEngWheel;
}

void MainOpenglWidget::setWheelCollision(const Model &model)
{
    auto drawable = m_renderer.makeDrawable<WireframeMesh>( model, QColor( 0, 255, 0 ) );
    m_leftSteerWheel->setDrawable( drawable );
    m_rightSteerWheel->setDrawable( drawable );
    m_leftEngWheel->setDrawable( drawable );
    m_rightEngWheel->setDrawable( drawable );
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
        Vector3D currentCameraLocation = m_scene->getCameraNode()->getLocation();
        currentCameraLocation.setZ( currentCameraLocation.z() + ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        m_scene->getCameraNode()->setLocation( currentCameraLocation );
    }
    else if( leftPressed )
    {
//        Vector3D camera_rotation_now = m_scene->getCameraNode()->getRotation();
        Vector3D camera_rotation_now = m_scene->getRotationCameraNode()->getRotation();
        Vector3D camera_rotation_now2 = m_scene->getCameraNode()->getRotation();
        camera_rotation_now.setZ( camera_rotation_now.z() + ( leftClickPos.x() - event->x() ) / 5 );
        camera_rotation_now2.setY( camera_rotation_now2.y() + ( leftClickPos.x() - event->x() ) / 5 );
        auto cameraLocationNow = m_scene->getCameraNode()->getLocation();
//        cameraLocationNow.setX( cameraLocationNow.x() + qCos( qDegreesToRadians( -camera_rotation_now.z() ) ) * ( leftClickPos.y() - event->y() ) * 10 );
//        cameraLocationNow.setY( cameraLocationNow.y() + qSin( qDegreesToRadians( -camera_rotation_now.z() ) ) * ( leftClickPos.y() - event->y() ) * 10 );
        cameraLocationNow.setX( cameraLocationNow.x() + ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
//        m_scene->getCamera().setOrientation( camera_rotation_now );
        m_scene->getCameraNode()->setLocation( cameraLocationNow );
        m_scene->getRotationCameraNode()->setRotation( camera_rotation_now );
//        m_scene->getCameraNode()->setRotation( camera_rotation_now2 );
    }
    else if( rightPressed )
    {
        Vector3D camera_rotation_now = m_scene->getCameraNode()->getRotation();
        camera_rotation_now.setZ( camera_rotation_now.z() - ( rightClickPos.x() - event->x() ) / 5 );
        camera_rotation_now.setY( camera_rotation_now.y() - ( rightClickPos.y() - event->y() ) / 5 );
        auto c = cursor();
        c.setPos( mapToGlobal( rightClickPos ) );
        setCursor(c);
        m_scene->getCameraNode()->setRotation( camera_rotation_now );
    }
}
