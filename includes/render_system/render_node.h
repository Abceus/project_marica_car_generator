#pragma once

#include "render_system/drawable.h"
#include "render_system/scene_node.h"
#include <QSharedPointer>

struct RenderNode
{
    SceneNode* m_node;
    Drawable* m_drawable;

    bool operator ==(const RenderNode& other) const
    {
        return m_drawable == other.m_drawable && m_node == other.m_node;
    }
};
