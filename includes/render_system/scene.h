#pragma once

#include "resources/model.h"
#include "object.h"
#include "physics/physobject.h"
#include "render_system/scene_node.h"

#include <QSharedPointer>
#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

class Scene
{
public:
    Scene();
    ~Scene() = default;

    void init();

    void clear();

    QVector3D getCameraLocation();
    void setCameraLocation( QVector3D value );

    QVector3D getCameraRotation();
    void setCameraRotation( QVector3D value );

    float getCameraScale();
    void setCameraScale( float value );

    void draw( QOpenGLFunctions* f, QOpenGLExtraFunctions* ef );
    void resizeScreen( int w, int h );

    QSharedPointer<SceneNode> addNode( QSharedPointer<SceneNode> newNode );

private:
    void drawNode( QSharedPointer<SceneNode> node, QOpenGLFunctions* f, QOpenGLExtraFunctions* ef );
    QVector3D camera_location;
    QVector3D camera_rotation;
    float camera_scale;

    QSharedPointer<SceneNode> m_rootNode;

    QOpenGLShaderProgram m_shaderProgram;
    QMatrix4x4 m_projection;
};
