#pragma once

#include <memory>

#include "render_system/mesh.h"
#include "render_system/scene_node.h"

class Object
{
public:
    Object( QSharedPointer<Mesh> model, QSharedPointer<SceneNode> node );
    ~Object() = default;

    QSharedPointer<SceneNode> getNode();
    QSharedPointer<Mesh> getDraweable();
protected:
    QSharedPointer<SceneNode> m_node;
    QSharedPointer<Mesh> m_model;
};
