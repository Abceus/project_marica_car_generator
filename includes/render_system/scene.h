#pragma once

#include "resources/model.h"
#include "object.h"
#include "render_system/scene_node.h"
#include "render_system/render_node.h"
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

    void draw( QOpenGLFunctions* f, QOpenGLExtraFunctions* ef );
    void resizeScreen( int w, int h );

    QSharedPointer<SceneNode> addNode( QSharedPointer<SceneNode> newNode );

    QSharedPointer<SceneNode> getCameraNode() const;
    QSharedPointer<SceneNode> getRotationCameraNode() const;
    Camera &getCamera();

private:
//    void drawNode( QSharedPointer<SceneNode> node, QOpenGLFunctions* f, QOpenGLExtraFunctions* ef );

    QSharedPointer<SceneNode> m_rootNode;

    //TODO: rework
    Camera m_camera;
    QSharedPointer<SceneNode> m_rotationCameraNode;
    QSharedPointer<SceneNode> m_cameraNode;

    QSharedPointer<QOpenGLShaderProgram> m_shaderProgram;
    QSharedPointer<QOpenGLShaderProgram> m_defaultShaderProgram;

    QVector<RenderNode> m_renderNodes;

    void addNodeToRenders( SceneNode* node );

private slots:
    void drawableAdded( SceneNode* node, Drawable* drawable );
    void drawableRemoved( SceneNode* node, Drawable* drawable );
    void nodeAdded( SceneNode* node );
    void nodeRemoved( SceneNode* node );
};
