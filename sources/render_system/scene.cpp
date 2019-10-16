#include "render_system/scene.h"
#include <QOpenGLShader>

Scene::Scene()
    : camera_scale( 1.f )
    , m_shaderProgram()
    , m_rootNode( QSharedPointer<SceneNode>( new SceneNode ) )
{
}

void Scene::init()
{
    QOpenGLShader vertexShader( QOpenGLShader::Vertex );

    bool success = vertexShader.compileSourceFile( "resources/shaders/defaultvertexshader.vert" );

    if( !success )
    {
        qDebug() << "The vertex shader wasn't compiled";
    }

    QOpenGLShader fragmentShader( QOpenGLShader::Fragment );
    success = fragmentShader.compileSourceFile( "resources/shaders/defaultfragmentshader.frag" );

    if( !success )
    {
        qDebug() << "The fragment shader wasn't compiled";
    }

    m_shaderProgram.addShader( &vertexShader );
    m_shaderProgram.addShader( &fragmentShader );
    if( !m_shaderProgram.link() )
    {
        qDebug() << "The shader program wasn't linked";
    }

    camera_location = QVector3D( 0, 0, 0 );
    camera_rotation = QVector3D( 0, 0, 0 );
    camera_scale = 1;
}

QVector3D Scene::getCameraLocation()
{
    return camera_location;
}

void Scene::setCameraLocation( QVector3D value )
{
    camera_location = value;
}

QVector3D Scene::getCameraRotation()
{
    return camera_rotation;
}

void Scene::setCameraRotation( QVector3D value )
{
    camera_rotation = value;
}

float Scene::getCameraScale()
{
    return camera_scale;
}

void Scene::setCameraScale( float value )
{
    camera_scale = value;
}

//QSharedPointer<Object> Scene::addObject(QSharedPointer<Object> newObject)
//{
//    if( m_objects.indexOf(newObject) == -1 )
//    {
//        m_objects.append( newObject );
//    }
//    return newObject;
//}

//void Scene::removeObject(QSharedPointer<Object> removeObject)
//{
//    m_objects.removeOne( removeObject );
//}

void Scene::draw( QOpenGLFunctions* f, QOpenGLExtraFunctions* ef )
{
    m_shaderProgram.bind();

    QMatrix4x4 view;
    QQuaternion rotation = QQuaternion::fromEulerAngles( getCameraRotation() );
    view.rotate( rotation );
    view.translate( getCameraLocation() );

    m_shaderProgram.setUniformValue( m_shaderProgram.uniformLocation( "view" ), view );
    m_shaderProgram.setUniformValue( m_shaderProgram.uniformLocation( "projection" ), m_projection );

    drawNode( m_rootNode, f, ef );
}

void Scene::resizeScreen(int w, int h)
{
    // Calculate aspect ratio
    const float aspect = static_cast<float>( w ) / static_cast<float>( h ? h : 1 );
    const float zNear = 1.f, zFar = 10000.f, fov = 90.f;

    // Reset projection
    m_projection.setToIdentity();

    // Set perspective projection
    m_projection.perspective( fov, aspect, zNear, zFar );
}

QSharedPointer<SceneNode> Scene::addNode(QSharedPointer<SceneNode> newNode)
{
    m_rootNode->addChild( newNode );
    return newNode;
}

void Scene::drawNode(QSharedPointer<SceneNode> node, QOpenGLFunctions *f, QOpenGLExtraFunctions *ef)
{
    QMatrix4x4 model;
    model.translate( node->getLocation() );
    model.rotate( QQuaternion::fromEulerAngles( node->getRotation() ) );
    model.scale( node->getScale() );

    m_shaderProgram.setUniformValue( m_shaderProgram.uniformLocation( "model" ), model );

    for( auto i = node->drawableBegin(); i != node->drawableEnd(); i++ )
    {
        (*i)->draw({f, ef, &m_shaderProgram});
    }

    for( const auto& childNode: *node )
    {
        //        drawNode( childNode, parentMatrix, f, ef );
        drawNode( childNode, f, ef );
    }
}
