#include "openglsimulationwidget.h"
#include "render_system/wireframe.h"
#include "render_system/box.h"

#include "errorsystem.h"

OpenglSimulationWidget::OpenglSimulationWidget( QWidget *parent )
        : QOpenGLWidget( parent )
        , physicWorld( new PhysicWorld() )
        , m_scene( new Scene )
        , prevTime( std::chrono::high_resolution_clock::now() )
        , m_body( nullptr )
        , m_leftSteerWheel( nullptr )
        , m_rightSteerWheel( nullptr )
        , m_leftEngWheel( nullptr )
        , m_rightEngWheel( nullptr )
        , pHinge2( nullptr )
        , m_testBlyat( 0.0f )
        , m_GLinited( false )
{
}

bool OpenglSimulationWidget::prepare( const Model &bodyModel, const Model &bodyPhysModel, QSharedPointer<SceneNode> nodeInformation, const Model &wheelModel,
                                      QSharedPointer<SceneNode> leftSteerNode, QSharedPointer<SceneNode> rightSteerNode, QSharedPointer<SceneNode> leftEngNode, QSharedPointer<SceneNode> rightEngNode )
{
//    makeCurrent();
//    auto node = m_scene->addNode( QSharedPointer<SceneNode>( new SceneNode ) );
//    node->setLocation( nodeInformation->getLocation() );
//    node->setRotation( nodeInformation->getRotation() );
//    node->setScale( nodeInformation->getScale() );

//    auto cameraOffset = node->addChild( QSharedPointer<SceneNode>( new SceneNode ) );
//    cameraOffset->setLocation( Vector3D( -300.0f, 0.0f, 300.0f ) );
//    cameraOffset->addChild( m_scene->getCameraNode() );

//    auto drawable = node->addDrawable( QSharedPointer<Mesh>( new Mesh( bodyModel ) ) );
//    node->addDrawable( QSharedPointer<WireframeMesh>( new WireframeMesh( bodyPhysModel, QColor( 0, 255, 0 ) ) ) );

//    m_body = QSharedPointer<PhysObject>( new PhysObject( drawable, node, bodyPhysModel, 1000.f ) );
//    m_body->setPhysic( physicWorld->addBody( m_body->getConstructionInfo() ) );

//    auto wheelDrawable = node->addDrawable( QSharedPointer<WireframeMesh>( QSharedPointer<WireframeMesh>( new WireframeMesh( wheelModel, QColor( 0, 255, 0 ) ) ) ) );

//    auto localLeftSteerNode = m_scene->addNode( QSharedPointer<SceneNode>( new SceneNode ) );
//    QMatrix4x4 leftSteerRotationMatrix;
//    leftSteerRotationMatrix.rotate( QQuaternion::fromEulerAngles( leftSteerNode->getParentRotation().getQtVector() ) );
//    localLeftSteerNode->setLocation( leftSteerRotationMatrix.mapVector( leftSteerNode->getLocation().getQtVector() ) );
//    localLeftSteerNode->setRotation( leftSteerNode->getRotation() );
//    localLeftSteerNode->setScale( leftSteerNode->getScale() );
//    localLeftSteerNode->addDrawable( wheelDrawable );
//    m_leftSteerWheel = QSharedPointer<PhysObject>( new PhysObject( wheelDrawable, localLeftSteerNode, wheelModel, 1000.f ) );
//    m_leftSteerWheel->setPhysic( physicWorld->addBody( m_leftSteerWheel->getConstructionInfo() ) );

//    auto localRightSteerNode = m_scene->addNode( QSharedPointer<SceneNode>( new SceneNode ) );
//    QMatrix4x4 rightSteerRotationMatrix;
//    rightSteerRotationMatrix.rotate( QQuaternion::fromEulerAngles( rightSteerNode->getParentRotation().getQtVector() ) );
//    localRightSteerNode->setLocation( rightSteerRotationMatrix.mapVector( rightSteerNode->getLocation().getQtVector() ) );
//    localRightSteerNode->setRotation( rightSteerNode->getRotation() );
//    localRightSteerNode->setScale( rightSteerNode->getScale() );
//    localRightSteerNode->addDrawable( wheelDrawable );
//    m_rightSteerWheel = QSharedPointer<PhysObject>( new PhysObject( wheelDrawable, localRightSteerNode, wheelModel, 1000.f ) );
//    m_rightSteerWheel->setPhysic( physicWorld->addBody( m_rightSteerWheel->getConstructionInfo() ) );

//    auto localLeftEngNode = m_scene->addNode( QSharedPointer<SceneNode>( new SceneNode ) );
//    QMatrix4x4 leftEngRotationMatrix;
//    leftEngRotationMatrix.rotate( QQuaternion::fromEulerAngles( leftEngNode->getParentRotation().getQtVector() ) );
//    localLeftEngNode->setLocation( leftEngRotationMatrix.mapVector( leftEngNode->getLocation().getQtVector() ) );
//    localLeftEngNode->setRotation( leftEngNode->getRotation() );
//    localLeftEngNode->setScale( leftEngNode->getScale() );
//    localLeftEngNode->addDrawable( wheelDrawable );
//    m_leftEngWheel = QSharedPointer<PhysObject>( new PhysObject( wheelDrawable, localLeftEngNode, wheelModel, 1000.f ) );
//    m_leftEngWheel->setPhysic( physicWorld->addBody( m_leftEngWheel->getConstructionInfo() ) );

//    auto localRightEngNode = m_scene->addNode( QSharedPointer<SceneNode>( new SceneNode ) );
//    QMatrix4x4 rightEngRotationMatrix;
//    rightEngRotationMatrix.rotate( QQuaternion::fromEulerAngles( rightEngNode->getParentRotation().getQtVector() ) );
//    localRightEngNode->setLocation( rightEngRotationMatrix.mapVector( rightEngNode->getLocation().getQtVector() ) );
//    localRightEngNode->setRotation( rightEngNode->getRotation() );
//    localRightEngNode->setScale( rightEngNode->getScale() );
//    localRightEngNode->addDrawable( wheelDrawable );
//    m_rightEngWheel = QSharedPointer<PhysObject>( new PhysObject( wheelDrawable, localRightEngNode, wheelModel, 1000.f ) );
//    m_rightEngWheel->setPhysic( physicWorld->addBody( m_rightEngWheel->getConstructionInfo() ) );

//    btVector3 parentAxis(0.f, 1.f, 0.f);
//    btVector3 childAxis(1.f, 0.f, 0.f);
//    btVector3 anchor = btVector3( localRightEngNode->getLocation().x(), localRightEngNode->getLocation().y(), localRightEngNode->getLocation().z() );
//    pHinge2 = new btHinge2Constraint( *(m_body->getPhysic().get()), *(m_rightEngWheel->getPhysic().get()), anchor, parentAxis, childAxis);

//    // add constraint to world
//    physicWorld->addConstraint(pHinge2, true);

//    // Drive engine.
//    pHinge2->enableMotor(3, true);
//    pHinge2->setMaxMotorForce(3, 1000);
//    pHinge2->setTargetVelocity(3, 0);

//    // Steering engine.
//    pHinge2->enableMotor(5, true);
//    pHinge2->setMaxMotorForce(5, 1000);
//    pHinge2->setTargetVelocity(5, 0);

//    pHinge2->setParam( BT_CONSTRAINT_CFM, 0.15f, 2 );
//    pHinge2->setParam( BT_CONSTRAINT_ERP, 0.35f, 2 );

//    pHinge2->setDamping( 2, 2.0 );
//    pHinge2->setStiffness( 2, 40.0 );

//    pHinge2->setDbgDrawSize(btScalar(5.f));

//    m_objects.append( physicWorld );
//    m_objects.append( m_body );
//    m_objects.append( m_leftSteerWheel );
//    m_objects.append( m_rightSteerWheel );
//    m_objects.append( m_leftEngWheel );
//    m_objects.append( m_rightEngWheel );

////    m_rightEngWheel->getPhysic()->applyImpulse( btVector3( 10.0f, 0.0f, 0.0f ), btVector3() );

//    auto meshVertexShader = m_renderer.loadShader( "resources/shaders/texturevertexshader.vert", QOpenGLShader::Vertex );

//    if( !meshVertexShader )
//    {
//        ErrorSystem::showError("The mesh vertex shader wasn't compiled", true);
//        return false;
//    }

//    auto meshFragmentShader = m_renderer.loadShader( "resources/shaders/texturefragmentshader.frag", QOpenGLShader::Fragment );

//    if( !meshFragmentShader )
//    {
//        ErrorSystem::showError("The mesh fragment shader wasn't compiled", true);
//        return false;
//    }

//    auto meshProgram = m_renderer.getShaderProgram( meshVertexShader, meshFragmentShader );

//    if( !meshProgram )
//    {
//        ErrorSystem::showError("The mesh program wasn't compiled", true);
//        return false;
//    }

//    m_body->getNode()->setShaderProgram( meshProgram );

//    auto groundNode = m_scene->addNode( QSharedPointer<SceneNode>( new SceneNode ) );
//    groundNode->setLocation( nodeInformation->getLocation() + Vector3D( 0.0f, 0.0f, -200.0f ) );
//    groundNode->addDrawable( m_renderer.makeDrawable<Box>( Vector3D( 2000.0f, 50.0f, 2000.0f ), QColor( 255.0f, 255.0f, 0.0f ) ) );

//    btBoxShape* colShapeGround = new btBoxShape( btVector3( 2000.f, 50.f, 2000.f ) );

//    btTransform startTransformGround;
//    startTransformGround.setIdentity();
//    btScalar massGround( 0.f );
//    bool isDynamicGround = ( massGround != 0.f );

//    btVector3 localInertiaGround(0, 0, 0);
//    if ( isDynamicGround )
//    {
//        colShapeGround->calculateLocalInertia( massGround, localInertiaGround );
//    }

//    startTransformGround.setOrigin(btVector3(
//            btScalar( 0.0 ),
//            btScalar( -200.0 ),
//            btScalar( 0.0 ) ) );

//    auto myMotionStateGround = new btDefaultMotionState( startTransformGround );

//    btRigidBody::btRigidBodyConstructionInfo rbInfoGround( massGround, myMotionStateGround, colShapeGround, localInertiaGround );

//    physicWorld->addBody( rbInfoGround );
//    return true;
    return false;
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
    m_body = nullptr;
    m_leftEngWheel = nullptr;
    m_rightEngWheel = nullptr;
    m_leftSteerWheel = nullptr;
    m_rightSteerWheel = nullptr;
    physicWorld->init();
    m_scene->clear();
}

bool OpenglSimulationWidget::isGLinited() const
{
    return m_GLinited;
}

void OpenglSimulationWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor( 0.2f, 0.3f, 0.3f, 1.0f );

    // Enable depth buffer
    f->glEnable( GL_DEPTH_TEST );

    f->glEnable( GL_BLEND );
    f->glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    m_renderer = Renderer( context(), context()->surface() );
    m_renderer.init();

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

//    if( !program )
//    {
//        ErrorSystem::showError("The program  wasn't compiled", true);
//        return;
//    }

//    m_scene->init( program );

    doneCurrent();
    m_GLinited = true;
}

void OpenglSimulationWidget::paintGL()
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
//    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear( GL_COLOR_BUFFER_BIT );
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1> >>( now - prevTime ).count();
    prevTime = now;

    static int test = 0;
    test++;
    if( pHinge2 && test > 100 )
    {
        m_testBlyat += 0.1f * dt;
        m_rightEngWheel->getPhysic()->setLinearVelocity( btVector3( 10.0f, 0.0f, 100.0f ) + m_rightEngWheel->getPhysic()->getAngularVelocity() );
//        pHinge2->setTargetVelocity( 3, m_testBlyat );
        test = 0;
    }

    for( auto& object: m_objects )
    {
        object->update( dt );
    }

    m_renderer.draw( m_scene );

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
    m_scene->resizeScreen( w, h );
}

void OpenglSimulationWidget::keyPressEvent( QKeyEvent* event )
{
    keys[event->key()] = true;
}

void OpenglSimulationWidget::keyReleaseEvent( QKeyEvent* event )
{
    keys[event->key()] = false;
}
