#include "openglsimulationwidget.h"

OpenglSimulationWidget::OpenglSimulationWidget( QWidget *parent )
        : QOpenGLWidget( parent )
        , scene()
        , prevTime( std::chrono::high_resolution_clock::now() )
        , physicWorld( new PhysicWorld() )
{
}

void OpenglSimulationWidget::rewriteThisShit( const QString &filename )
{
    makeCurrent();
    auto node = scene.addNode( QSharedPointer<SceneNode>( new SceneNode ) );
    node->setLocation( { 0.0f, 0.0f, -300.0f } );
    auto drawable = node->addDrawable( QSharedPointer<Mesh>( new Mesh( Model::readPSK( filename ) ) ) );
    m_tire = QSharedPointer<PhysObject>( new PhysObject( drawable.staticCast<Mesh>(), node, 1000.f, QVector3D( 10.f, 3.f, 10.f ) ) );

    m_tire->setPhysic( physicWorld->addBody( m_tire->getConstructionInfo() ) );

    m_objects.append( physicWorld );
    m_objects.append( m_tire );

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
            btScalar( -350.0 ) ) );

    auto myMotionStateGround = new btDefaultMotionState( startTransformGround );

    btRigidBody::btRigidBodyConstructionInfo rbInfoGround( massGround, myMotionStateGround, colShapeGround, localInertiaGround );

    physicWorld->addBody( rbInfoGround );
}

void OpenglSimulationWidget::closeEvent(QCloseEvent *event)
{
    QOpenGLWidget::closeEvent(event);
    init();
    emit closed();
}

void OpenglSimulationWidget::init()
{
    m_objects.clear();
    m_tire = nullptr;
    physicWorld->init();
    scene.clear();
}

void OpenglSimulationWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );

    // Enable depth buffer
    f->glEnable( GL_DEPTH_TEST );

    f->glEnable( GL_BLEND );
    f->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

//    scene.init();

    makeCurrent();
}

void OpenglSimulationWidget::paintGL()
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear( GL_COLOR_BUFFER_BIT );
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1> >>( now - prevTime ).count();
    prevTime = now;

    for( auto& object: m_objects )
    {
        object->update( dt );
    }

    scene.draw( f, ef );

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
    scene.resizeScreen( w, h );
}

void OpenglSimulationWidget::keyPressEvent( QKeyEvent* event )
{
    keys[event->key()] = true;
}

void OpenglSimulationWidget::keyReleaseEvent( QKeyEvent* event )
{
    keys[event->key()] = false;
}
