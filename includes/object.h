#pragma once

#include <memory>

#include "render_system/drawable.h"
#include "render_system/mesh.h"
#include "render_system/scene_node.h"

class Object
{
public:
    Object( QSharedPointer<Drawable> model, QSharedPointer<SceneNode> node );
    ~Object() = default;

    QSharedPointer<SceneNode> getNode();
    QSharedPointer<Drawable> getDrawable();

    void setDrawable(const QSharedPointer<Drawable>& newModel);
protected:
    QSharedPointer<SceneNode> m_node;
    QSharedPointer<Drawable> m_model;
};
