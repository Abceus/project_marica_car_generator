#include "render_system/camera.h"

Camera::Camera()
    : m_projection()
{

}

void Camera::resizeScreen(int w, int h)
{
    // Calculate aspect ratio
    const float aspect = static_cast<float>( w ) / static_cast<float>( h ? h : 1 );
    const float zNear = 1.f, zFar = 10000.f, fov = 90.f;

    // Reset projection
    m_projection.setToIdentity();

    // Set perspective projection
    m_projection.perspective( fov, aspect, zNear, zFar );
}

QMatrix4x4 Camera::getProjection() const
{
    return m_projection;
}
