#include <algorithm>
#include <QtMath>

#include "mainopenglwidget.h"
#include "render_system/wireframe.h"
#include "render_system/box.h"
#include "errorsystem.h"
#include "resources/resource_manager.h"

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

//    m_body->getNode()->setLocation(QVector3D(0.0f, -150.0f, -400.0f));
//    m_body->getNode()->setRotation(Vector3D(0.0f, 0.0f, 90.0f));

    m_camera = QSharedPointer<Camera>( new Camera );
    m_camera->setNode( m_scene->addNode( QSharedPointer<SceneNode>( new SceneNode ) ) );
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
    m_renderer.init();
    m_renderer.setMainCamera( m_camera );

//    auto vertexShader = m_renderer.loadShader( "resources/shaders/colorvertexshader.vert", QOpenGLShader::Vertex );

//    if( !vertexShader )
//    {
//        ErrorSystem::showError("The vertex shader wasn't compiled", true);
//        return;
//    }

//    auto fragmentShader = m_renderer.loadShader( "resources/shaders/colorfragmentshader.frag", QOpenGLShader::Fragment );

//    if( !fragmentShader )
//    {
//        ErrorSystem::showError("The fragment shader wasn't compiled", true);
//        return;
//    }

//    auto program = m_renderer.getShaderProgram( vertexShader, fragmentShader );

//    m_scene->init( program );

//    auto meshVertexShader = m_renderer.loadShader( "resources/shaders/texturevertexshader.vert", QOpenGLShader::Vertex );

//    if( !meshVertexShader )
//    {
//        ErrorSystem::showError("The mesh vertex shader wasn't compiled", true);
//        return;
//    }

//    auto meshFragmentShader = m_renderer.loadShader( "resources/shaders/texturefragmentshader.frag", QOpenGLShader::Fragment );

//    if( !meshFragmentShader )
//    {
//        ErrorSystem::showError("The mesh fragment shader wasn't compiled", true);
//        return;
//    }

//    auto meshProgram = m_renderer.getShaderProgram( meshVertexShader, meshFragmentShader );

    doneCurrent();
}

void MainOpenglWidget::paintGL()
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
//    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear( GL_COLOR_BUFFER_BIT );

    m_renderer.draw( m_scene );

    update();
}

void MainOpenglWidget::resizeGL( int w, int h )
{
    m_scene->resizeScreen( w, h );
    m_camera->resizeScreen( w, h );
}

void MainOpenglWidget::changeMeshModel( ResourcePointer<Model> model )
{
    makeCurrent();

    auto drawable = m_renderer.makeDrawable<Mesh>( model );
    m_body->setDrawable( drawable.staticCast<Mesh>() );
}

void MainOpenglWidget::changeBodyModel( ResourcePointer<Model> model )
{
    makeCurrent();

    auto drawable = m_renderer.makeDrawable<WireframeMesh>( model );
    m_collisionBody->setDrawable( drawable.staticCast<WireframeMesh>() );
}

void MainOpenglWidget::changeTireModel( ResourcePointer<Model> model )
{
    makeCurrent();

    auto drawable = m_renderer.makeDrawable<WireframeMesh>( model );
    m_leftSteerWheel->setDrawable( drawable.staticCast<WireframeMesh>() );
    m_rightSteerWheel->setDrawable( drawable.staticCast<WireframeMesh>() );
    m_leftEngWheel->setDrawable( drawable.staticCast<WireframeMesh>() );
    m_rightEngWheel->setDrawable( drawable.staticCast<WireframeMesh>() );
}

void MainOpenglWidget::changeSkin( size_t index, ResourcePointer<QImage> texture )
{
    Material material;
    material.texture = ResourceManager::Instance().getTextureManager().get(texture.getPath());
    m_body->getDrawable().staticCast<Mesh>()->setMaterial( index, material );
}

void MainOpenglWidget::setModel( QSharedPointer<ProjectModel> newModel )
{
    if( m_model != newModel )
    {
        if( m_model )
        {
            disconnect(m_model.get(), &ProjectModel::bodyMeshChanged, this, &MainOpenglWidget::changeMeshModel);
            disconnect(m_model.get(), &ProjectModel::bodyCollisionChanged, this, &MainOpenglWidget::changeBodyModel);
            disconnect(m_model.get(), &ProjectModel::tireCollisionChanged, this, &MainOpenglWidget::changeTireModel);
            disconnect(m_model.get(), &ProjectModel::skinChanged, this, &MainOpenglWidget::changeSkin);
        }

        m_model = newModel;

        connect(m_model.get(), &ProjectModel::bodyMeshChanged, this, &MainOpenglWidget::changeMeshModel);
        connect(m_model.get(), &ProjectModel::bodyCollisionChanged, this, &MainOpenglWidget::changeBodyModel);
        connect(m_model.get(), &ProjectModel::tireCollisionChanged, this, &MainOpenglWidget::changeTireModel);
        connect(m_model.get(), &ProjectModel::skinChanged, this, &MainOpenglWidget::changeSkin);
    }
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
        Vector3D currentCameraLocation = m_camera->getLocation();
        currentCameraLocation.setZ( currentCameraLocation.z() + ( leftClickPos.y() - event->y() ) * 10 );
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        m_camera->setLocation( currentCameraLocation );
    }
    else if( leftPressed )
    {
        Vector3D camera_rotation_now = m_camera->getRotation();
        camera_rotation_now.setZ( camera_rotation_now.z() + ( leftClickPos.x() - event->x() ) / 5 );
        auto cameraLocationNow = m_camera->getLocation();

        Vector3D lookAt(( leftClickPos.y() - event->y() ) * 10, 0.0f, 0.0f);
        QMatrix4x4 rotateMatrix;
        rotateMatrix.rotate(QQuaternion::fromEulerAngles(camera_rotation_now.getQtVector()));
        auto c = cursor();
        c.setPos( mapToGlobal( leftClickPos ) );
        setCursor(c);
        m_camera->setLocation( cameraLocationNow + rotateMatrix * lookAt.getQtVector() );
    }
    else if( rightPressed )
    {
        Vector3D camera_rotation_now = m_camera->getRotation();
        camera_rotation_now.setZ( camera_rotation_now.z() - ( rightClickPos.x() - event->x() ) / 5 );
        camera_rotation_now.setY( camera_rotation_now.y() - ( rightClickPos.y() - event->y() ) / 5 );
        auto c = cursor();
        c.setPos( mapToGlobal( rightClickPos ) );
        setCursor(c);
        m_camera->setRotation( camera_rotation_now );
    }
}
