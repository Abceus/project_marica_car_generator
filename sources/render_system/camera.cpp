#include "render_system/camera.h"

#include <QtMath>
#include <QVector3D>
#include <QMatrix4x4>

Camera::Camera()
    : m_projection()
{

}

void Camera::resizeScreen(int w, int h)
{
    // Calculate aspect ratio
    const float aspect = static_cast<float>( w  ) / static_cast<float>( h ? h : 1 );
    const float zNear = 10.f, zFar = 5000.f, fov = 90.f;

    // Reset projection
    m_projection.setToIdentity();

    // Set perspective projection
    m_projection.perspective( fov, aspect, zNear, zFar );
}

QMatrix4x4 Camera::getProjection() const
{
    QMatrix4x4 eye;
    Vector3D lookAt(1.0f, 0.0f, 0.0f);
    Vector3D lookFrom = m_location;
    QMatrix4x4 rotateMatrix;
    rotateMatrix.rotate(QQuaternion::fromEulerAngles(m_rotation.getQtVector()));
    lookAt = m_location + rotateMatrix * lookAt.getQtVector();

    if(m_baseNode)
    {
        lookAt = m_baseNode->getMatrix() * lookAt.getQtVector();
        lookFrom = m_baseNode->getMatrix() * lookFrom.getQtVector();
    }
    eye.lookAt(lookFrom.getQtVector(), lookAt.getQtVector(), Vector3D(0.0f, 0.0f, 1.0f).getQtVector());
    return m_projection * eye;
}

void Camera::setNode( QSharedPointer<SceneNode> node )
{
    m_baseNode = node;
}

QSharedPointer<SceneNode> Camera::getNode() const
{
    return m_baseNode;
}

void Camera::setRotation( const Vector3D& rotation )
{
    m_rotation = rotation;
}

Vector3D Camera::getRotation() const
{
    return m_rotation;
}

void Camera::setLocation( const Vector3D& location )
{
    m_location = location;
}

Vector3D Camera::getLocation() const
{
    return m_location;
}
