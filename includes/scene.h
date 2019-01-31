#pragma once

#include <memory>

#include "model.h"
#include "object.h"
#include "physobject.h"

class Scene
{
public:
    Scene();
    ~Scene() = default;

    void setBodyObject( Object *object );
    Object* getBodyObject() const;

    void setTireCollision( PhysObject* tireCollision );
    PhysObject* getTireCollision();

    QVector3D getCameraLocation();
    void setCameraLocation( QVector3D value );

    QVector3D getCameraRotation();
    void setCameraRotation( QVector3D value );

    float getCameraScale();
    void setCameraScale( float value );
private:
    QVector3D camera_location;
    QVector3D camera_rotation;
    float camera_scale;
    std::unique_ptr<Object> body;
    std::unique_ptr<PhysObject> m_tireCollision;
};
