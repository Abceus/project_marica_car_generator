#pragma once

#include <QOpenGLTexture>

#include "resources/resource_pointer.h"

enum BlendType
{
    BT_None,
    BT_Alpha
};

struct Material
{
    ResourcePointer<QOpenGLTexture> texture = nullptr;
    BlendType blendType = BT_None;
    QColor color = QColor(255, 255, 255, 255);
};
