#include "render_system/scene.h"
#include <QOpenGLShader>
#include <QVector3D>
#include <QFile>

#include <algorithm>

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

    struct FaceCollection
    {
        RenderNode renderNode;
        QVector<Face> faces;
    };

    struct Shit
    {
        RenderNode* rn;
        QVector<Vertex>* v;
        QVector<bool>* vc;
        QVector<float>* vcv;
        Face* f;
        float getDistance( const QVector3D& cameraLocation ) const
        {
            QVector<float> distances;
            distances.resize(3);
            for( size_t i = 0; i < 3; i++ )
            {
                if( vc->at( f->points[i] ) )
                {
                    distances[i] = vcv->at( f->points[i] );
                }
                else
                {
                    auto t = QVector3D( v->at( f->points[i] ).X, v->at( f->points[i] ).Y, v->at( f->points[i] ).Z );
                    t = rn->m_node->getMatrix().map( t );
//                    auto t2 = t + rn->m_node->getLocation();
//                    distances[i] = cameraLocation.distanceToPoint( QVector3D( v->at( f->points[i] ).X, v->at( f->points[i] ).Y, v->at( f->points[i] ).Z ) + rn->m_node->getLocation() );

                    distances[i] = std::fabs( cameraLocation.x() -  t.x() ) + std::fabs( cameraLocation.y() -  t.y() ) + std::fabs( cameraLocation.z() -  t.z() );
                    (*vc)[f->points[i]] = true;
                    (*vcv)[f->points[i]] = distances[i];
                }
            }
            return std::min( std::min( distances[0], distances[1] ), distances[2] );
        }
    };

    QVector<FaceCollection> transparentFaces;
    QVector<QVector<Vertex>> vertexes;
    QVector<QVector<bool>> vertexesCalculated;
    QVector<QVector<float>> vertexesCalculatedValues;

    QList<Shit> shits;

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

        transparentFaces.append( { renderNode, drawable->getTransparentFaces() } );
        vertexes.append( drawable->getVertexes() );
        vertexesCalculated.append( QVector<bool>() );
        vertexesCalculated.back().resize( drawable->getVertexes().size() );
        vertexesCalculated.back().fill( false );
        vertexesCalculatedValues.append( QVector<float>() );
        vertexesCalculatedValues.back().resize( drawable->getVertexes().size() );
    }

    for( size_t i = 0; i < transparentFaces.size(); i++ )
    {
       auto& transparentFace = transparentFaces[i];
       for( auto& face: transparentFace.faces )
       {
           shits.append( { &(transparentFace.renderNode), &(vertexes[i]), &(vertexesCalculated[i]), &(vertexesCalculatedValues[i]), &face } );
       }
    }

    std::sort( shits.begin(), shits.end(), [=]( const Shit& a, const Shit& b ) {
        auto aDistance = a.getDistance( camera_location );
        auto bDistance = b.getDistance( camera_location );
        if( aDistance == bDistance )
        {
            if( a.rn->m_node == b.rn->m_node )
            {
                if( a.rn->m_drawable == b.rn->m_drawable )
                {
                    return a.f->indiceIndex < b.f->indiceIndex;
                }
                return a.rn->m_drawable < b.rn->m_drawable;
            }
            return a.rn->m_node < b.rn->m_node;
        }
        return aDistance < bDistance;
    } );

    QFile file("./test.txt");

    if( !shits.empty() )
    {
        file.open( QIODevice::WriteOnly );
    }

    QTextStream stream(&file);

    int row = 1;
    int startIndex = 0;
//    for( const auto& shit: shits )
    for( size_t i = 0; i < shits.size(); ++i )
    {
        const auto& shit = shits[i];
//        stream << shit.rn->m_drawable << " " << shit.f->indiceIndex << " " << shit.getDistance( camera_location ) << '\n';
//        if( shit.rn->m_drawable == d && shit.f->indiceIndex - prevIndex == 1 )
//        {
//            row++;
//            prevIndex = shit.f->indiceIndex;
//            d = shit.rn->m_drawable;
//            continue;
//        }

        if( row == 1 )
        {
            startIndex = shit.f->indiceIndex;
        }

        if( i+1 < shits.size() && shit.rn->m_drawable == shits[i+1].rn->m_drawable && shit.rn->m_node == shits[i+1].rn->m_node && shits[i+1].f->indiceIndex - shit.f->indiceIndex == 1 )
        {
            row++;
            continue;
        }

        auto node = shit.rn->m_node;
        auto drawable = shit.rn->m_drawable;

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

        drawable->subdraw( {f, ef, m_shaderProgram.get()}, shit.f->materialIndex, startIndex, row );

        row = 1;
    }

    if( !shits.empty() )
    {
        file.close();
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
