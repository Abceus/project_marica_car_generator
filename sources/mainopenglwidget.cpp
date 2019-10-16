#include <algorithm>
#include <QtMath>

#include "mainopenglwidget.h"

MainOpenglWidget::MainOpenglWidget( QWidget *parent )
    : QOpenGLWidget( parent )
    , scene()
{
    setMouseTracking( true );
    leftPressed = false;
    rightPressed = false;
    setCursor( Qt::CrossCursor );
}

void MainOpenglWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor( 0.25f, 0.25f, 0.25f, 1.0f );

    // Enable depth buffer
    f->glEnable( GL_DEPTH_TEST );

    f->glEnable( GL_BLEND );
    f->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    scene.init();
    makeCurrent();
}

void MainOpenglWidget::paintGL()
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear( GL_COLOR_BUFFER_BIT );

    scene.draw( f, ef );

    update();
}

void MainOpenglWidget::resizeGL( int w, int h )
{
    scene.resizeScreen( w, h );
}

void MainOpenglWidget::setBodyObject( QSharedPointer<Object> object )
{
//    if( m_body )
//    {
//        scene.removeObject( m_body );
//    }
//    m_body = scene.addObject( object );
    m_body = object;
}

//void MainOpenglWidget::setTireCollision( PhysObject *physObject )
//{
//    scene->setTireCollision( std::shared_ptr<PhysObject>( physObject ) );
//}

Object* MainOpenglWidget::getBodyObject() const
{
    return m_body.get();
}

void MainOpenglWidget::setBodyTexture( const QString &filename, size_t index )
{
    m_body->getDraweable()->setTexture( filename, index );
}

Scene &MainOpenglWidget::getScene()
{
    // TODO: rework
    makeCurrent();
    return scene;
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
        QVector3D currentCameraLocation = scene.getCameraLocation();
        currentCameraLocation.setY( currentCameraLocation.y() - ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        scene.setCameraLocation( currentCameraLocation );
    }
    else if( leftPressed )
    {
        QVector3D camera_rotation_now = scene.getCameraRotation();
        camera_rotation_now.setY( camera_rotation_now.y() - ( leftClickPos.x() - event->x() ) / 5 );
        auto cameraLocationNow = scene.getCameraLocation();
        cameraLocationNow.setZ( cameraLocationNow.z() + qCos( qDegreesToRadians( camera_rotation_now.y() ) ) * ( leftClickPos.y() - event->y() ) * 10 );
        cameraLocationNow.setX( cameraLocationNow.x() - qSin( qDegreesToRadians( camera_rotation_now.y() ) ) * ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        scene.setCameraRotation( camera_rotation_now );
        scene.setCameraLocation( cameraLocationNow );
    }
    else if( rightPressed )
    {
        QVector3D camera_rotation_now = scene.getCameraRotation();
        camera_rotation_now.setY( camera_rotation_now.y() - ( rightClickPos.x() - event->x() ) / 5 );
        camera_rotation_now.setX( camera_rotation_now.x() - ( rightClickPos.y() - event->y() ) / 5 );
        auto c = cursor();
        c.setPos( mapToGlobal( rightClickPos ) );
        setCursor(c);
        scene.setCameraRotation( camera_rotation_now );
    }
}

std::unique_ptr<Mesh> MainOpenglWidget::makeModel( const QString &filename )
{
    makeCurrent();
    return std::make_unique<Mesh>( Model::readPSK( filename ) );
}
