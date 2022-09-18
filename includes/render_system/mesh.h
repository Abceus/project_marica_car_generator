#pragma once

#include <memory>
#include <string>
#include <vcruntime.h>

#include "resources/unanimation.h"
#include "resources/model.h"
#include "render_system/drawable.h"
#include "render_system/texture.h"
#include "render_system/batch.h"
#include "utils/math/vec3.h"
#include "utils/range_array.h"

class Mesh : public Drawable
{
public:
    void init(const Model& model);
    void setTexture( const std::shared_ptr<Texture>& texture, size_t index );
    void addTexture( const std::shared_ptr<Texture>& texture );
    void draw(ShaderProgram* shaderProgram) const override;

    void rotateBone(const std::string& boneName, const Quaternion& quaternion);
    void transposeBone(const std::string& boneName, const Vec3f& position);
private:
    std::vector<std::unique_ptr<Batch>> batches;
    Model model;

    std::map<Vertex*, std::vector<Bone*>> vlinks;
    std::map<Bone*, std::vector<std::pair<Vertex*, float>>> blinks;
    std::map<Vertex*, size_t> vmap;

    mutable RangeArray<Vertex> updates;
};
