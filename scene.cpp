#include "scene.h"

void Scene::setBodyModel(std::unique_ptr<Model> model)
{
    body = std::move(model);
    camera_location = QVector3D(0,0,0);
    camera_rotation = QVector3D(0,0,0);
    camera_scale = 1;
}

Model* Scene::getBodyModel()
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
