#pragma once

#include <memory>

#include "resources/unanimation.h"
#include "resources/model.h"
#include "render_system/drawable.h"
#include "render_system/texture.h"
#include "render_system/batch.h"

class Mesh : public Drawable
{
public:
    void init(const Model& model);
    void setTexture( const std::shared_ptr<Texture>& texture, size_t index );
    void addTexture( const std::shared_ptr<Texture>& texture );
    void draw(ShaderProgram* shaderProgram) const override;
private:
    std::vector<std::unique_ptr<Batch>> batches;
};
