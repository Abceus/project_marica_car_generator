#include "render_system/batch.h"

Batch::Batch(const std::shared_ptr<TriangleArray>& vertexArray,
             const std::shared_ptr<Texture>& texture)
    : Batch(vertexArray, texture, 0, vertexArray->getSize()) {}

Batch::Batch(const std::shared_ptr<TriangleArray>& vertexArray,
             const std::shared_ptr<Texture>& texture, size_t firstIndex,
             size_t count)
    : vertexArray(vertexArray), texture(texture), firstIndex(firstIndex),
      count(count) {}

void Batch::draw() const {
    texture->activate();
    vertexArray->draw(firstIndex, count);
    texture->deactivate();
}

void Batch::setTexture(const std::shared_ptr<Texture>& texture_) {
    texture = texture_;
}

std::weak_ptr<TriangleArray> Batch::getVertexArray() const {
    return vertexArray;
}