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
#include <QString>
#include <QWheelEvent>
#include <QMatrix4x4>
#include <QSharedPointer>

#include "resources/model.h"
#include "render_system/scene.h"
#include "object.h"
#include "render_system/renderer.h"

class MainOpenglWidget :public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MainOpenglWidget( QWidget *parent = nullptr );
    ~MainOpenglWidget() override = default;

    void mousePressEvent( QMouseEvent *event ) override;
    void mouseMoveEvent( QMouseEvent *event ) override;
    void mouseReleaseEvent( QMouseEvent *event ) override;

    void setBodyObject( QSharedPointer<Object> object );
    Object* getBodyObject() const;
    void setBodyTexture( const QString &filename, size_t index );

    QSharedPointer<Scene> getScene();
    Renderer& getRenderer();
private:
    QSharedPointer<Scene> m_scene;
    Renderer m_renderer;
    QSharedPointer<Object> m_body;
    QPoint leftClickPos;
    bool leftPressed;
    QPoint rightClickPos;
    bool rightPressed;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
