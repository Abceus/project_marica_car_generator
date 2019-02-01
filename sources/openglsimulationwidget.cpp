#include "openglsimulationwidget.h"

OpenglSimulationWidget::OpenglSimulationWidget( QWidget *parent )
        : QOpenGLWidget( parent )
        , scene( std::make_unique<Scene>() )
        , prevTime( std::chrono::high_resolution_clock::now() )
{
}


void OpenglSimulationWidget::rewriteThisShit( const QString &filename )
{
//    scene->setBodyObject( new Object( makeModel( filename ) ) );
    scene->setTireCollision( new PhysObject( std::move( makeModel( filename ) ), 0.f, 0.f, -300.f, 1000.f, QVector3D( 10.f, 3.f, 10.f ) ) );

    scene->getTireCollision()->setPhysic( physicWorld.addBody( scene->getTireCollision()->getConstructionInfo() ) );

    btBoxShape* colShapeGround = new btBoxShape( btVector3( 20.f, 50.f, 20.f ) );

    btTransform startTransformGround;
    startTransformGround.setIdentity();
    btScalar massGround( 0.f );
    bool isDynamicGround = ( massGround != 0.f );

    btVector3 localInertiaGround(0, 0, 0);
    if ( isDynamicGround )
    {
        colShapeGround->calculateLocalInertia( massGround, localInertiaGround );
    }

    startTransformGround.setOrigin(btVector3(
            btScalar( 5.0 ),
            btScalar( -150.0 ),
            btScalar( -300.0 ) ) );

    auto myMotionStateGround = new btDefaultMotionState( startTransformGround );

    btRigidBody::btRigidBodyConstructionInfo rbInfoGround( massGround, myMotionStateGround, colShapeGround, localInertiaGround );

    physicWorld.addBody( rbInfoGround );
}

std::unique_ptr<Mesh> OpenglSimulationWidget::makeModel( const QString &filename )
{
    makeCurrent();
    return std::make_unique<Mesh>( Model::readPSK( filename ) );
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
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1> >>( now - prevTime ).count();
    prevTime = now;
    physicWorld.update( dt );

    if( scene->getTireCollision() != nullptr )
    {
//        Object *object = scene->getBodyObject();
        PhysObject *physObject = scene->getTireCollision();
        QMatrix4x4 model;
        auto pos = physObject->getPosition();
        model.translate( physObject->getPosition() );
        model.rotate( physObject->getRotation() );
        ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "model" ), model );
//        GLint current_vao;
//        f->glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
        physObject->getModel()->bindVAO();
//        f->glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &current_vao);
        for( size_t i=0; i<physObject->getModel()->getTexturesSize(); i++ )
        {
            size_t index = physObject->getModel()->getTextureQueue( i );
            physObject->getModel()->bindTexture( i );
            ShaderProgram->setUniformValue( "texture", 0 );
            ShaderProgram->setUniformValue( ShaderProgram->uniformLocation( "nowTexture" ), static_cast<GLint>( index ) );
            f->glDrawElements( GL_TRIANGLES, physObject->getModel()->getVAOsize(), GL_UNSIGNED_INT, nullptr );
        }
        physObject->getModel()->releaseVAO();
    }

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
