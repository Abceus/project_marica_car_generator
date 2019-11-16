#pragma once

#include <QMatrix4x4>

#include "vector3d.h"

class Camera
{
public:
    Camera();
    void resizeScreen( int w, int h );
    QMatrix4x4 getProjection() const;
private:
    QMatrix4x4 m_projection;
};
