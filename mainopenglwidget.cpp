#include <algorithm>

#include "mainopenglwidget.h"

MainOpenglWidget::MainOpenglWidget(QWidget *parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
    leftPressed = false;
    rightPressed = false;
}

MainOpenglWidget::~MainOpenglWidget()
{
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    ef->glDeleteVertexArrays(1, &VAO);
    f->glDeleteBuffers(1, &VBO);
    f->glDeleteBuffers(1, &EBO);
}

void MainOpenglWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Enable depth buffer
    f->glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    //f->glEnable(GL_CULL_FACE);

    std::unique_ptr<QOpenGLShader> vertexShader(new QOpenGLShader(QOpenGLShader::Vertex));

    bool success = false;

    #ifdef QT_NO_DEBUG
        success = vertexShader->compileSourceFile("./defaultvertexshader.vert");
    #else
        success = vertexShader->compileSourceFile("./../ProjectMaricaCarGenerator/defaultvertexshader.vert");
    #endif

    if(!success)
    {
        qDebug() << "Вершинный шейдер не скомпилировался";
    }

    std::unique_ptr<QOpenGLShader> fragmentShader (new QOpenGLShader(QOpenGLShader::Fragment));

    #ifdef QT_NO_DEBUG
        success = fragmentShader->compileSourceFile("./defaultfragmentshader.frag");
    #else
        success = fragmentShader->compileSourceFile("./../ProjectMaricaCarGenerator/defaultfragmentshader.frag");
#endif

    if(!success)
    {
        qDebug() << "Фрагментный шейдер не скомпилировался";
    }

    ShaderProgram = std::unique_ptr<QOpenGLShaderProgram>(new QOpenGLShaderProgram());
    ShaderProgram->addShader(vertexShader.get());
    ShaderProgram->addShader(fragmentShader.get());
    if(!ShaderProgram->link())
    {
        qDebug() << "Программа не линканулась";
    }

    ShaderProgram->bind();

    scene = Scene();
}

void MainOpenglWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT);


    QMatrix4x4 transMat(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);


    QMatrix4x4 view;
    QMatrix4x4 model;
    QVector3D z = scene.getCameraLocation();
    z.setY(0);
    z.setX(0);
    view.translate(z);
    QQuaternion rotation = QQuaternion::fromEulerAngles(scene.getCameraRotation());
    view.rotate(rotation);
    QVector3D xy = scene.getCameraLocation();
    xy.setZ(0);
    view.translate(xy);

    ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("model"), model);
    ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("view"), view);
    ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("projection"), projection);

    f->glUseProgram(ShaderProgram->programId());

    if(scene.getBodyModel() != nullptr)
    {
        ef->glBindVertexArray(scene.getBodyModel()->VAO);
        f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

    ef->glBindVertexArray(0);

    update();
}

void MainOpenglWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 100.0, fov = 45.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}


void MainOpenglWidget::setBodyModel(std::unique_ptr<Model> model)
{
    scene.setBodyModel(std::move(model));
}

void MainOpenglWidget::wheelEvent(QWheelEvent *event)
{
    QVector3D now_camera_location = scene.getCameraLocation();
    now_camera_location.setZ(now_camera_location.z() + event->delta()/1000.0);
    scene.setCameraLocation(now_camera_location);
}

void MainOpenglWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        leftClickPos = event->pos();
        leftPressed = true;
    }
    else if(event->button() == Qt::RightButton)
    {
        rightClickPos = event->pos();
        rightPressed = true;
    }
}

void MainOpenglWidget::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        leftPressed = false;
    }
    else if(event->button() == Qt::RightButton)
    {
        rightPressed = false;
    }
}

void MainOpenglWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(leftPressed)
    {
        QVector3D camera_rotation_now = scene.getCameraRotation();
        camera_rotation_now.setY(camera_rotation_now.y() + (leftClickPos.x() - event->x()));
        camera_rotation_now.setX(camera_rotation_now.x() + (leftClickPos.y() - event->y()));
        leftClickPos = event->pos();
        scene.setCameraRotation(camera_rotation_now);
    }
    else if(rightPressed)
    {
        QVector3D camera_location_now = scene.getCameraLocation();
        camera_location_now.setY(camera_location_now.y() + (rightClickPos.y() - event->y())/100.0);
        camera_location_now.setX(camera_location_now.x() - (rightClickPos.x() - event->x())/100.0);
        rightClickPos = event->pos();
        scene.setCameraLocation(camera_location_now);
    }
}

std::unique_ptr<Model> MainOpenglWidget::getModel(QString filename)
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    return std::unique_ptr<Model>(new Model(filename, f, ef));
}
