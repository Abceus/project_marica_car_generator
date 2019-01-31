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

#include "model.h"
#include "scene.h"
#include "object.h"

class MainOpenglWidget :public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit MainOpenglWidget( QWidget *parent = nullptr );
    ~MainOpenglWidget() override = default;

    void wheelEvent(QWheelEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

    void setBodyObject(Object *object);
    std::unique_ptr<Mesh> makeModel( const QString &filename );
    Object* getBodyObject() const;
    void setBodyTexture( const QString &filename, size_t index );
private:
    std::unique_ptr<Scene> scene;
    QPoint leftClickPos;
    bool leftPressed;
    QPoint rightClickPos;
    bool rightPressed;
    QMatrix4x4 projection;
    std::unique_ptr<QOpenGLShaderProgram> ShaderProgram;
protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
};
