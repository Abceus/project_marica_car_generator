#include <algorithm>
#include <QtMath>

#include "mainopenglwidget.h"

MainOpenglWidget::MainOpenglWidget( QWidget *parent )
    : QOpenGLWidget( parent )
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

    std::unique_ptr<QOpenGLShader> vertexShader( new QOpenGLShader( QOpenGLShader::Vertex ) );

    bool success = vertexShader->compileSourceFile( "resources/shaders/defaultvertexshader.vert" );

    if( !success )
    {
        qDebug() << "The vertex shader wasn't compiled";
    }

    std::unique_ptr<QOpenGLShader> fragmentShader( new QOpenGLShader(QOpenGLShader::Fragment ) );
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

    scene = std::make_shared<Scene>();
    makeCurrent();
}

void MainOpenglWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClear( GL_COLOR_BUFFER_BIT );

    ShaderProgram->bind();

    QMatrix4x4 view;
    QQuaternion rotation = QQuaternion::fromEulerAngles( scene->getCameraRotation() );
    view.rotate( rotation );
    view.translate( scene->getCameraLocation() );

    ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "view" ), view );
    ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "projection" ), projection );

    if( scene->getBodyObject() != nullptr )
    {
        Object *object = scene->getBodyObject();
        QMatrix4x4 model;
        model.translate( object->getPosition() );
        ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "model" ), model );

//        GLint current_vao;
//        f->glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
        scene->getBodyObject()->getModel()->bindVAO();
//        f->glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
        for( size_t i=0; i<object->getModel()->getTexturesSize(); i++ )
        {
            size_t index = object->getModel()->getTextureQueue( i );
            object->getModel()->bindTexture( i );
            ShaderProgram->setUniformValue( "texture", 0 );
            ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "nowTexture" ), static_cast<GLint>( index ) );
            f->glDrawElements( GL_TRIANGLES, object->getModel()->getVAOsize(), GL_UNSIGNED_INT, nullptr );
        }
        scene->getBodyObject()->getModel()->releaseVAO();
    }
    update();
}

void MainOpenglWidget::resizeGL( int w, int h )
{
    // Calculate aspect ratio
    const float aspect = static_cast<float>( w ) / static_cast<float>( h ? h : 1 );
    const float zNear = 1.f, zFar = 10000.f, fov = 90.f;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective( fov, aspect, zNear, zFar );
}

void MainOpenglWidget::setBodyObject( Object *object )
{
    scene->setBodyObject( object );
}

//void MainOpenglWidget::setTireCollision( PhysObject *physObject )
//{
//    scene->setTireCollision( std::shared_ptr<PhysObject>( physObject ) );
//}

Object* MainOpenglWidget::getBodyObject() const
{
    return scene->getBodyObject();
}

void MainOpenglWidget::setBodyTexture( const QString &filename, size_t index )
{
    scene->getBodyObject()->getModel()->setTexture( filename, index );
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
        QVector3D currentCameraLocation = scene->getCameraLocation();
        currentCameraLocation.setY( currentCameraLocation.y() - ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        scene->setCameraLocation( currentCameraLocation );
    }
    else if( leftPressed )
    {
        QVector3D camera_rotation_now = scene->getCameraRotation();
        camera_rotation_now.setY( camera_rotation_now.y() - ( leftClickPos.x() - event->x() ) / 5 );
        auto cameraLocationNow = scene->getCameraLocation();
        cameraLocationNow.setZ( cameraLocationNow.z() + qCos( qDegreesToRadians( camera_rotation_now.y() ) ) * ( leftClickPos.y() - event->y() ) * 10 );
        cameraLocationNow.setX( cameraLocationNow.x() - qSin( qDegreesToRadians( camera_rotation_now.y() ) ) * ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        scene->setCameraRotation( camera_rotation_now );
        scene->setCameraLocation( cameraLocationNow );
    }
    else if( rightPressed )
    {
        QVector3D camera_rotation_now = scene->getCameraRotation();
        camera_rotation_now.setY( camera_rotation_now.y() - ( rightClickPos.x() - event->x() ) / 5 );
        camera_rotation_now.setX( camera_rotation_now.x() - ( rightClickPos.y() - event->y() ) / 5 );
        auto c = cursor();
        c.setPos( mapToGlobal( rightClickPos ) );
        setCursor(c);
        scene->setCameraRotation( camera_rotation_now );
    }
}

std::unique_ptr<Mesh> MainOpenglWidget::makeModel( const QString &filename )
{
    makeCurrent();
    return std::make_unique<Mesh>( Model::readPSK( filename ) );
}
