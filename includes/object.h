#pragma once

#include <memory>

#include "render_system/mesh.h"
#include "render_system/scene_node.h"

class Object
{
public:
    Object( QSharedPointer<Drawable> model = nullptr, QSharedPointer<SceneNode> node = nullptr );
    ~Object() = default;

    QSharedPointer<SceneNode> getNode();
    QSharedPointer<Drawable> getDrawable();

    void setNode( QSharedPointer<SceneNode> node );
    void setDrawable( QSharedPointer<Drawable> drawable );
protected:
    QSharedPointer<SceneNode> m_node;
    QSharedPointer<Drawable> m_drawable;
};
