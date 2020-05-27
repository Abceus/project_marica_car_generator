#pragma once

#include <QMatrix4x4>
#include <QSharedPointer>

#include "vector3d.h"
#include "render_system/scene_node.h"

class Camera
{
public:
    Camera();
    void resizeScreen( int w, int h );
    QMatrix4x4 getProjection() const;

    void setNode( QSharedPointer<SceneNode> node );
    QSharedPointer<SceneNode> getNode() const;

    void setRotation( const Vector3D& rotation );
    Vector3D getRotation() const;

    void setLocation( const Vector3D& location );
    Vector3D getLocation() const;
private:
    QMatrix4x4 m_projection;
    QSharedPointer<SceneNode> m_baseNode;
    Vector3D m_location;
    Vector3D m_rotation;
};
