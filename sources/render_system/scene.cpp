#include "render_system/scene.h"
#include <QOpenGLShader>

#include "algorithm"

Scene::Scene()
    : camera_scale( 1.f )
    , m_shaderProgram(nullptr)
    , m_defaultShaderProgram(nullptr)
    , m_rootNode( QSharedPointer<SceneNode>( new SceneNode ) )
{
    connect( m_rootNode.get(), &SceneNode::nodeAdded, this, &Scene::nodeAdded );
    connect( m_rootNode.get(), &SceneNode::nodeRemoved, this, &Scene::nodeRemoved );
    connect( m_rootNode.get(), &SceneNode::drawableAdded, this, &Scene::drawableAdded );
    connect( m_rootNode.get(), &SceneNode::drawableRemoved, this, &Scene::drawableRemoved );
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

void Scene::draw( QOpenGLFunctions* f, QOpenGLExtraFunctions* ef )
{
//    drawNode( m_rootNode, f, ef );

    for( const auto& renderNode: m_renderNodes )
    {
        auto node = renderNode.m_node;
        auto drawable = renderNode.m_drawable;

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

        drawable->draw( {f, ef, m_shaderProgram.get()} );
    }
    if( m_shaderProgram )
    {
        m_shaderProgram->release();
        m_shaderProgram = nullptr;
    }
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
        drawNode( childNode, f, ef );
    }
}

void Scene::addNodeToRenders(SceneNode *node)
{
    for( auto i = node->drawableBegin(); i != node->drawableEnd(); i++ )
    {
        m_renderNodes.append( RenderNode { node, i->get() } );
    }

    for( const auto& child: *node )
    {
        addNodeToRenders( &(*child) );
    }
}

void Scene::drawableAdded(SceneNode *node, Drawable *drawable)
{
    if( !m_renderNodes.contains( { node, drawable } ) )
    {
        m_renderNodes.append( { node, drawable } );
    }
}

void Scene::drawableRemoved(SceneNode *node, Drawable *drawable)
{
    m_renderNodes.removeAll( { node, drawable } );
}

void Scene::nodeAdded(SceneNode *node)
{
    addNodeToRenders( node );
}

void Scene::nodeRemoved(SceneNode *node)
{
    auto removeIterator = std::remove_if( m_renderNodes.begin(), m_renderNodes.end(), [node]( const RenderNode& a ){ return a.m_node == node; });
    m_renderNodes.erase( removeIterator, m_renderNodes.end() );
}
