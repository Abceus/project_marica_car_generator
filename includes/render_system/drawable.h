#pragma once

#include "render_system/shader_program.h"

class Drawable {
public:
    virtual void draw(ShaderProgram* shaderProgram) const = 0;
    virtual ~Drawable() = default;
};
