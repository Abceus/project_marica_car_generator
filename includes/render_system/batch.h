#pragma once
#include "render_system/texture.h"
#include "render_system/vertex_array.h"
#include "utils/gl.h"
#include <memory>

class Batch {
public:
    Batch(const std::shared_ptr<VertexArray>& vertexArray,
          const std::shared_ptr<Texture>& texture);

    Batch(const std::shared_ptr<VertexArray>& vertexArray,
          const std::shared_ptr<Texture>& texture, size_t firstIndex,
          size_t count);

    void draw() const;

    void setTexture(const std::shared_ptr<Texture>& texture);
private:
    std::shared_ptr<VertexArray> vertexArray;
    std::shared_ptr<Texture> texture;
    size_t firstIndex;
    size_t count;
};