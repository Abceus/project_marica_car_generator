#pragma once

#include "resources/model.h"
#include "render_system/batching.h"
#include <QVector>
#include "vector3d.h"

class SceneNode;

class Drawable
{
public:
    Drawable();
    virtual ~Drawable() = default;
    virtual DrawBuffers getDrawBuffers() const = 0;
    virtual void setEnable( bool value );
    virtual bool getEnable() const;
    virtual void setParent( SceneNode* parent );
    virtual SceneNode* getParent() const;

    virtual Vector3D getLocation();
    virtual Vector3D getOriginLocation() const;

    virtual Vector3D getRotation() const;
    virtual Vector3D getOriginRotation() const;

    virtual Scale3D getScale() const;
    virtual Scale3D getOriginScale() const;

    virtual QMatrix4x4 getOriginMatrix() const;
    virtual QMatrix4x4 getMatrix() const;
protected:
    bool m_enable;
    SceneNode* m_parent;
};
