#include "scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
    QOpenGLExtraFunctions *ef = QOpenGLContext::currentContext()->extraFunctions();
    QOpenGLFunctions *f = QOpenGLContext::currentContext()->functions();
    ef->glDeleteVertexArrays(1, &(this->body->getModel()->VAO));
    f->glDeleteBuffers(1, &(this->body->getModel()->VBO));
    f->glDeleteBuffers(1, &(this->body->getModel()->EBO));
}

void Scene::setBodyObject(Object *body)
{
    this->body = std::unique_ptr<Object>(body);
    camera_location = QVector3D(0,0,0);
    camera_rotation = QVector3D(0,0,0);
    camera_scale = 1;
}

Object* Scene::getBodyObject()
{
    return body.get();
}

QVector3D Scene::getCameraLocation()
{
    return camera_location;
}

void Scene::setCameraLocation(QVector3D value)
{
    camera_location = value;
}

QVector3D Scene::getCameraRotation()
{
    return camera_rotation;
}

void Scene::setCameraRotation(QVector3D value)
{
    camera_rotation = value;
}

float Scene::getCameraScale()
{
    return camera_scale;
}

void Scene::setCameraScale(float value)
{
    camera_scale = value;
}
