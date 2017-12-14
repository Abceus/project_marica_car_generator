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
}

void MainOpenglWidget::initializeGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    f->glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Enable depth buffer
    f->glEnable(GL_DEPTH_TEST);

    f->glEnable(GL_BLEND);
    f->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::unique_ptr<QOpenGLShader> vertexShader(new QOpenGLShader(QOpenGLShader::Vertex));

    bool success = false;

    #ifdef QT_NO_DEBUG
        success = vertexShader->compileSourceFile("./defaultvertexshader.vert");
    #else
        success = vertexShader->compileSourceFile("./../ProjectMaricaCarGenerator/defaultvertexshader.vert");
    #endif

    if(!success)
    {
        qDebug() << "The vertex shader wasn't compiled";
    }

    std::unique_ptr<QOpenGLShader> fragmentShader (new QOpenGLShader(QOpenGLShader::Fragment));

    #ifdef QT_NO_DEBUG
        success = fragmentShader->compileSourceFile("./defaultfragmentshader.frag");
    #else
        success = fragmentShader->compileSourceFile("./../ProjectMaricaCarGenerator/defaultfragmentshader.frag");
    #endif

    if(!success)
    {
        qDebug() << "The fragment shader wasn't compiled";
    }

    ShaderProgram = std::unique_ptr<QOpenGLShaderProgram>(new QOpenGLShaderProgram());
    ShaderProgram->addShader(vertexShader.get());
    ShaderProgram->addShader(fragmentShader.get());
    if(!ShaderProgram->link())
    {
        qDebug() << "The shader program wasn't linked";
    }

    ShaderProgram->bind();

    scene = std::unique_ptr<Scene>(new Scene());
}

void MainOpenglWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT);


    ShaderProgram->bind();

    QMatrix4x4 view;
    QQuaternion rotation = QQuaternion::fromEulerAngles(scene->getCameraRotation());
    view.translate(scene->getCameraLocation());
    view.rotate(rotation);

    ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("view"), view);
    ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("projection"), projection);

    if(scene->getBodyObject() != nullptr)
    {
        Object *object = scene->getBodyObject();
        QMatrix4x4 model;
        model.translate(object->getX(), object->getY(), object->getZ());
        ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("model"), model);

        scene->getBodyObject()->getModel()->getVAO()->bind();
        for(int i=0; i<object->getModel()->getTexturesSize(); i++)
        {
            int index = object->getModel()->getTextureQueue(i);
            object->getModel()->getTexture(index)->bind();
            ShaderProgram->setUniformValue("texture", 0);
            ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("nowTexture"), index);
            f->glDrawElements(GL_TRIANGLES, object->getModel()->getVAOsize(), GL_UNSIGNED_INT, 0);
        }
        scene->getBodyObject()->getModel()->getVAO()->release();

        //object->getModel()->getTexture(0)->bind();
        //ShaderProgram->setUniformValue("texture", 0);

        //scene->getBodyObject()->getModel()->getVAO()->bind();
        //f->glDrawElements(GL_TRIANGLES, object->getModel()->getVAOsize(), GL_UNSIGNED_INT, 0);
        //scene->getBodyObject()->getModel()->getVAO()->release();
    }

    update();
}

void MainOpenglWidget::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    const qreal zNear = 1.0, zFar = 10000.0, fov = 90.0;

    // Reset projection
    projection.setToIdentity();

    // Set perspective projection
    projection.perspective(fov, aspect, zNear, zFar);
}


void MainOpenglWidget::setBodyObject(Object *object)
{
    scene->setBodyObject(object);
}

Object* MainOpenglWidget::getBodyObject() const
{
    return scene->getBodyObject();
}

void MainOpenglWidget::setBodyTexture(QString filename, int index)
{
    scene->getBodyObject()->getModel()->setTexture(filename, index);
}

void MainOpenglWidget::wheelEvent(QWheelEvent *event)
{
    QVector3D now_camera_location = scene->getCameraLocation();
    now_camera_location.setZ(now_camera_location.z() + event->delta()/100.0);
    scene->setCameraLocation(now_camera_location);
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
        QVector3D camera_rotation_now = scene->getCameraRotation();
        camera_rotation_now.setY(camera_rotation_now.y() + (leftClickPos.x() - event->x()));
        camera_rotation_now.setX(camera_rotation_now.x() + (leftClickPos.y() - event->y()));
        leftClickPos = event->pos();
        scene->setCameraRotation(camera_rotation_now);
    }
    else if(rightPressed)
    {
        QVector3D camera_location_now = scene->getCameraLocation();
        camera_location_now.setY(camera_location_now.y() + (rightClickPos.y() - event->y())/10.0);
        camera_location_now.setX(camera_location_now.x() - (rightClickPos.x() - event->x())/10.0);
        rightClickPos = event->pos();
        scene->setCameraLocation(camera_location_now);
    }
}

std::unique_ptr<Model> MainOpenglWidget::getModel(QString filename)
{
    makeCurrent();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    return std::unique_ptr<Model>(new Model(filename, f, ef));
}
