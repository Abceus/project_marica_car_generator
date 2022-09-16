#pragma once
#include "render_system/texture.h"
#include "render_system/triangle_array.h"
#include "utils/gl.h"
#include <memory>

class Batch {
public:
    Batch(const std::shared_ptr<TriangleArray>& vertexArray,
          const std::shared_ptr<Texture>& texture);

    Batch(const std::shared_ptr<TriangleArray>& vertexArray,
          const std::shared_ptr<Texture>& texture, size_t firstIndex,
          size_t count);

    void draw() const;

    void setTexture(const std::shared_ptr<Texture>& texture);

    std::weak_ptr<TriangleArray> getVertexArray() const;
private:
    std::shared_ptr<TriangleArray> vertexArray;
    std::shared_ptr<Texture> texture;
    size_t firstIndex;
    size_t count;
};