#pragma once

#include "render_system/render_info.h"

class Drawable
{
public:
    virtual void draw( const RenderInfo& renderInfo ) = 0;
};
