#include "render_system/scene.h"
#include <QOpenGLShader>

Scene::Scene()
    : camera_scale( 1.f )
    , m_shaderProgram(nullptr)
    , m_defaultShaderProgram(nullptr)
    , m_rootNode( QSharedPointer<SceneNode>( new SceneNode ) )
{
}

void Scene::init( QSharedPointer<QOpenGLShaderProgram> shaderProgram )
{
    m_defaultShaderProgram = shaderProgram;

    camera_location = QVector3D( 0, 0, 0 );
    camera_rotation = QVector3D( 0, 0, 0 );
    camera_scale = 1;
}

void Scene::clear()
{
    camera_location = QVector3D( 0, 0, 0 );
    camera_rotation = QVector3D( 0, 0, 0 );
    camera_scale = 1;

    m_rootNode->clear();
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
    drawNode( m_rootNode, f, ef );
    m_shaderProgram->release();
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
    bool shaderChanged = false;
    if( node->getShaderProgram() )
    {
        if( m_shaderProgram != node->getShaderProgram() )
        {
            if( m_shaderProgram )
            {
                m_shaderProgram->release();
            }
            m_shaderProgram = node->getShaderProgram();
            shaderChanged = true;
        }
    } else if( m_shaderProgram != m_defaultShaderProgram )
    {
        if( m_shaderProgram )
        {
            m_shaderProgram->release();
        }
        m_shaderProgram = m_defaultShaderProgram;
        shaderChanged = true;
    }

    if( shaderChanged )
    {
        m_shaderProgram->bind();

        QMatrix4x4 view;
        QQuaternion rotation = QQuaternion::fromEulerAngles( getCameraRotation() );
        view.rotate( rotation );
        view.translate( getCameraLocation() );

        m_shaderProgram->setUniformValue( m_shaderProgram->uniformLocation( "view" ), view );
        m_shaderProgram->setUniformValue( m_shaderProgram->uniformLocation( "projection" ), m_projection );
    }

    QMatrix4x4 model;
    model.translate( node->getLocation() );
    model.rotate( QQuaternion::fromEulerAngles( node->getRotation() ) );
    model.scale( node->getScale() );

    m_shaderProgram->setUniformValue( m_shaderProgram->uniformLocation( "model" ), model );

    for( auto i = node->drawableBegin(); i != node->drawableEnd(); i++ )
    {
        (*i)->draw({f, ef, m_shaderProgram.get()});
    }

    for( const auto& childNode: *node )
    {
        //        drawNode( childNode, parentMatrix, f, ef );
        drawNode( childNode, f, ef );
    }
}
