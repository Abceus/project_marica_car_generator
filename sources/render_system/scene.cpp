#include "render_system/scene.h"
#include "vector3d.h"

#include <QOpenGLShader>
#include <QFile>

#include <algorithm>
#include <cmath>

Scene::Scene()
    : m_rootNode( QSharedPointer<SceneNode>( new SceneNode ) )
{
}

void Scene::init( QSharedPointer<QOpenGLShaderProgram> shaderProgram )
{
}

void Scene::clear()
{
    m_rootNode->clear();
}

void Scene::draw( Batching& batchind, QSharedPointer<Camera> camera )
{
    drawNode( batchind, m_rootNode, camera->getProjection() );

//    std::cout << std::endl;
}

void Scene::drawNode( Batching& batching, QSharedPointer<SceneNode> node, QMatrix4x4 matrix )
{
    if(node->isEmpty()) return;

    if(node->getEnable())
    {
        auto buffers = node->getDrawBuffers();

        for(auto& buffer: buffers)
        {
            for(auto& vertex: buffer.vertices)
            {
                vertex.coords = matrix * vertex.coords;
            }
            batching.addElements(buffer);
        }
    }

//    auto currentMatrix = matrix * node->getOriginMatrix();
////    auto currentMatrix = matrix * node->getMatrix();

//    for(auto i = node->begin(); i != node->end(); i++)
//    {
//        if((*i)->getEnable())
//        {
//            auto buffers = (*i)->getDrawBuffers();

//            for(auto& buffer: buffers)
//            {
////                for(auto& vertex: buffer.vertices)
////                {
//////                    vertex = { currentMatrix.map(vertex.coords), vertex.textureCoords };
//////                    vertex = { currentMatrix * vertex.coords, vertex.textureCoords };
////                    vertex = { currentMatrix * vertex.coords, vertex.textureCoords };
//////                    std::cout << vertex.coords.x() << " " << vertex.coords.y() << " " << vertex.coords.z() << std::endl;
////                }

//                batching.addElements(buffer);
//            }
//        }
//    }
//    std::cout << "Node ended" << std::endl;

//    for(auto i = node->begin(); i != node->end(); i++)
//    {
//        drawNode(batching, i->staticCast<SceneNode>(), currentMatrix);
////        drawNode(batching, *i, matrix);
//    }
}

void Scene::resizeScreen(int w, int h)
{
}

QSharedPointer<SceneNode> Scene::addNode(QSharedPointer<SceneNode> newNode)
{
    m_rootNode->addChild( newNode );
    return newNode;
}
