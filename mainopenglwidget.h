#ifndef MAINOPENGLWIDGET_H
#define MAINOPENGLWIDGET_H

#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/glext.h>

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>
#include <QDebug>
#include <QOpenGLShaderProgram>
#include <QOpenGLShader>
#include <QString>
#include <QWheelEvent>
#include <QMatrix4x4>

#include <vector>
#include <memory>

#include "model.h"
#include "scene.h"

class MainOpenglWidget :public QOpenGLWidget
{
  Q_OBJECT
public:
    MainOpenglWidget(QWidget *parent = 0);
    ~MainOpenglWidget();

    std::unique_ptr<QOpenGLShaderProgram> ShaderProgram;
    GLuint VBO, VAO, EBO;
    QQuaternion rotation;

    void wheelEvent(QWheelEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    void setBodyModel(std::unique_ptr<Model> model);
    std::unique_ptr<Model> getModel(QString filename);

private:
    Scene scene;
    QPoint leftClickPos;
    bool leftPressed;

    QPoint rightClickPos;
    bool rightPressed;

    QMatrix4x4 projection;

protected:
    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

#endif // MAINOPENGLWIDGET_H
