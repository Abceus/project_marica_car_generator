#pragma once
#include "render_system/element_buffer.h"
#include "render_system/vertex_buffer.h"
#include "resources/model.h"
#include <memory>


class VertexArray {
public:
    void init(const std::shared_ptr<VertexBuffer>& vertexBuffer,
              const std::shared_ptr<ElementBuffer>& elementBuffer);
    void destroy();

    void bind() const;
    void unbind() const;

    bool isValid() const;

    size_t getSize() const;

    void draw() const;
    void draw(size_t firstIndex, size_t count) const;

private:
    std::shared_ptr<VertexBuffer> vertexBuffer;
    std::shared_ptr<ElementBuffer> elementBuffer;

    static constexpr GLuint INVALID_ARRAY_INDEX = 0;
    GLuint arrayIndex = INVALID_ARRAY_INDEX;
};