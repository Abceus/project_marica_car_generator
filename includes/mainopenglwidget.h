#pragma once

#include <vector>
#include <memory>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif
#include <GL/glu.h>
#include <GL/gl.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include <QString>
#include <QWheelEvent>
#include <QMatrix4x4>
#include <QSharedPointer>

#include "resources/model.h"
#include "render_system/scene.h"
#include "object.h"
#include "render_system/renderer.h"
#include "render_system/wireframe.h"
#include "project_model.h"

class MainOpenglWidget :public QOpenGLWidget
{
    Q_OBJECT
public slots:
    void changeMeshModel( ResourcePointer<Model> model );
    void changeBodyModel( CollisionObject model );
    void changeTireModel( CollisionObject model );

    void changeSkin( size_t index, ResourcePointer<QImage> texture );
public:
    explicit MainOpenglWidget( QWidget *parent = nullptr );
    ~MainOpenglWidget() override = default;

    void mousePressEvent( QMouseEvent *event ) override;
    void mouseMoveEvent( QMouseEvent *event ) override;
    void mouseReleaseEvent( QMouseEvent *event ) override;

    void setModel( QSharedPointer<ProjectModel> newModel );

private:
    QSharedPointer<Scene> m_scene;
    Renderer m_renderer;
    Object m_body;
    Object m_collisionBody;

    Object m_leftSteerWheel;
    Object m_rightSteerWheel;
    Object m_leftEngWheel;
    Object m_rightEngWheel;

    QSharedPointer<Camera> m_camera;

    QPoint leftClickPos;
    bool leftPressed;
    QPoint rightClickPos;
    bool rightPressed;

    QSharedPointer<ProjectModel> m_model;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
