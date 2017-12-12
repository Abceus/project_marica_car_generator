#ifndef SCENE_H
#define SCENE_H

#include "model.h"

#include <memory>

class Scene
{
public:
    void setBodyModel(std::unique_ptr<Model> model);
    Model* getBodyModel();
    QVector3D getCameraLocation();
    void setCameraLocation(QVector3D value);

    QVector3D getCameraRotation();
    void setCameraRotation(QVector3D value);

    float getCameraScale();
    void setCameraScale(float value);
private:
    //QVector<Model> models;
    QVector3D camera_location;
    QVector3D camera_rotation;
    float camera_scale;
    Model wheels;
    std::unique_ptr<Model> body;
    //Model body;
    Model car;
};

#endif // SCENE_H
