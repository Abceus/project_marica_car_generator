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

    ShaderProgram.release();
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
        success = vertexShader->compileSourceFile("defaultvertexshader.vert");
    #else
        success = vertexShader->compileSourceFile("C:/Users/Admin/Documents/ProgProjects/MaricaProjectCarGen2/MaricaProjectCarGen2/defaultvertexshader.vert");
    #endif

    if(!success)
    {
        qDebug() << "Вершинный шейдер не скомпилировался";
    }

    std::unique_ptr<QOpenGLShader> fragmentShader (new QOpenGLShader(QOpenGLShader::Fragment));

    #ifdef QT_NO_DEBUG
        success = fragmentShader->compileSourceFile("defaultfragmentshader.frag");
    #else
        success = fragmentShader->compileSourceFile("C:/Users/Admin/Documents/ProgProjects/MaricaProjectCarGen2/MaricaProjectCarGen2/defaultfragmentshader.frag");
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


    QMatrix4x4 transMat(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);
    //QQuaternion rotation = QQuaternion::fromAxisAndAngle(0, 0, 1, 5);

    //transMat.rotate(rotation);



    ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("qt_ModelViewProjectionMatrix"), transMat);
/*
    GLfloat vertices[] = {
         0.5f,  0.5f, 0.0f,  // Top Right
         0.5f, -0.5f, 0.0f,  // Bottom Right
        -0.5f, -0.5f, 0.0f,  // Bottom Left
        -0.5f,  0.5f, 0.0f  // Top Left
    };

    GLuint indices[] = {  // Note that we start from 0!
        0, 1, 3,  // First Triangle
        1, 2, 3   // Second Triangle
    };

    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();

    ef->glGenVertexArrays(1, &VAO);
    f->glGenBuffers(1, &VBO);
    f->glGenBuffers(1, &EBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    ef->glBindVertexArray(VAO);

    f->glBindBuffer(GL_ARRAY_BUFFER, VBO);
    f->glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    f->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    f->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    f->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    f->glEnableVertexAttribArray(0);

    f->glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind

    ef->glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO
*/

    scene = Scene();

    vertexShader.release();
    fragmentShader.release();

    //setMouseTracking(true);
}

void MainOpenglWidget::paintGL()
{
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    f->glClear(GL_COLOR_BUFFER_BIT);

    //ShaderProgram->bind();
    /*
    QMatrix4x4 transMat(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);

    rotation = QQuaternion::fromAxisAndAngle(0, 0, 1, 5) * rotation;

    transMat.rotate(rotation);

    ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("qt_ModelViewProjectionMatrix"), transMat);
    */

    QMatrix4x4 transMat(1, 0, 0, 0,
                         0, 1, 0, 0,
                         0, 0, 1, 0,
                         0, 0, 0, 1);

   // QMatrix4x4 transMat;
    transMat.translate(scene.getCameraLocation());

    QQuaternion rotation = QQuaternion::fromEulerAngles(scene.getCameraRotation());

    transMat.rotate(rotation);

    transMat.scale(scene.getCameraScale());

    //qDebug() << transMat;

    //transMat.perspective(360.0, 800.0 / 600.0, 1.0, 100.0);
    //transMat.viewport(100.0, 100.0, 100.0, 100.0);

    ShaderProgram->setUniformValue(ShaderProgram->uniformLocation("qt_ModelViewProjectionMatrix"), transMat);

    f->glUseProgram(ShaderProgram->programId());

    if(scene.getBodyModel() != nullptr)
    {
        ef->glBindVertexArray(scene.getBodyModel()->VAO);
        f->glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //qDebug() << "draw";
    }

    ef->glBindVertexArray(0);

    update();
}

void MainOpenglWidget::resizeGL(int width, int height)
{
}


void MainOpenglWidget::setBodyModel(std::unique_ptr<Model> model)
{
    scene.setBodyModel(std::move(model));
}

void MainOpenglWidget::wheelEvent(QWheelEvent *event)
{
    QVector3D now_camera_location = scene.getCameraLocation();
    scene.setCameraScale(std::max(scene.getCameraScale()+event->delta()/1000.0, 0.1));
    //now_camera_location.setX(now_camera_location.x() + event->delta()/1000.0);
    //qDebug() << now_camera_location;
    //scene.setCameraLocation(now_camera_location);
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
