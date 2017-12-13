#ifndef SCENE_H
#define SCENE_H

#include <memory>

#include "model.h"
#include "object.h"

class Scene
{
public:
    Scene();
    ~Scene();
    void setBodyObject(Object *object);
    Object* getBodyObject() const;
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
    std::unique_ptr<Object> wheels;
    std::unique_ptr<Object> body;
    //Model body;
    //Object car;
};

#endif // SCENE_H
