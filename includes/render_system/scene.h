#pragma once

#include "resources/model.h"
#include "object.h"
#include "render_system/scene_node.h"
#include "vector3d.h"
#include "render_system/camera.h"

#include <QSharedPointer>
#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
//#include <QObject>

class Scene : public QObject
{
public:
    Scene();
    ~Scene() = default;

    void init( QSharedPointer<QOpenGLShaderProgram> shaderProgram );

    void clear();

    void draw( Batching& batchind, QSharedPointer<Camera> camera );
    void drawNode( Batching& batchind, QSharedPointer<SceneNode> node, QMatrix4x4 matrix );
    void resizeScreen( int w, int h );

    QSharedPointer<SceneNode> addNode( QSharedPointer<SceneNode> newNode );

private:
    QSharedPointer<SceneNode> m_rootNode;
};
