#pragma once

#include <memory>

#include "resources/model.h"
#include "object.h"
#include "physics/physobject.h"

#include <QSharedPointer>
#include <QVector>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLExtraFunctions>

class Scene
{
public:
    Scene();
    ~Scene() = default;

    void init();

    void setTireCollision( PhysObject* tireCollision );
    PhysObject* getTireCollision();

    QVector3D getCameraLocation();
    void setCameraLocation( QVector3D value );

    QVector3D getCameraRotation();
    void setCameraRotation( QVector3D value );

    float getCameraScale();
    void setCameraScale( float value );

    QSharedPointer<Object> addObject( QSharedPointer<Object> newObject );
    void removeObject( QSharedPointer<Object> removeObject );

    void draw( QOpenGLFunctions* f, QOpenGLExtraFunctions* ef );
    void resizeScreen( int w, int h );

private:
    QVector3D camera_location;
    QVector3D camera_rotation;
    float camera_scale;
    QVector<QSharedPointer<Object>> m_objects;
    QOpenGLShaderProgram m_shaderProgram;
    QMatrix4x4 m_projection;

    std::unique_ptr<PhysObject> m_tireCollision;
};
