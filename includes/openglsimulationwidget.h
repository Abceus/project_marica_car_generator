#pragma once

#include <vector>
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif
#include <GL/glu.h>
#include <GL/gl.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QString>
#include <QWheelEvent>
#include <QMatrix4x4>

#include "physics/physicworld.h"
#include "physics/physobject.h"
#include "render_system/scene.h"
#include "updatable.h"
#include "render_system/renderer.h"

class OpenglSimulationWidget :public QOpenGLWidget
{
Q_OBJECT
public:
    explicit OpenglSimulationWidget( QWidget *parent = nullptr );
    ~OpenglSimulationWidget() override = default;
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;
    void prepare( const Model &bodyModel, const Model &bodyPhysModel, QSharedPointer<SceneNode> nodeInformation, const Model &wheelModel,
                  QSharedPointer<SceneNode> leftSteerNode, QSharedPointer<SceneNode> rightSteerNode, QSharedPointer<SceneNode> leftEngNode, QSharedPointer<SceneNode> rightEngNode );
    void closeEvent(QCloseEvent *event) override;
    void init();
private:
    QVector<QSharedPointer<IUpdatable>> m_objects;
    QMap<int, bool> keys;
    QSharedPointer<PhysicWorld> physicWorld;
    QSharedPointer<Scene> m_scene;
    Renderer m_renderer;
    std::chrono::time_point<std::chrono::high_resolution_clock> prevTime;

    QSharedPointer<PhysObject> m_body;

    QSharedPointer<PhysObject> m_leftSteerWheel;
    QSharedPointer<PhysObject> m_rightSteerWheel;
    QSharedPointer<PhysObject> m_leftEngWheel;
    QSharedPointer<PhysObject> m_rightEngWheel;
    btHinge2Constraint* pHinge2;
    btScalar m_testBlyat;
protected:
    void initializeGL() override;
    void resizeGL( int w, int h ) override;
    void paintGL() override;
signals:
    void closed();
};
