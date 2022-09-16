#include "render_system/triangle_array.h"
#include "render_system/vertex_buffer.h"
#include <memory>

void TriangleArray::init(const std::shared_ptr<VertexBuffer>& vertexBuffer_,
              const std::shared_ptr<ElementBuffer>& elementBuffer_) {
    if (isValid()) {
        destroy();
    }

    glGenVertexArrays(1, &arrayIndex);

    bind();

    vertexBuffer = vertexBuffer_;
    // vertexBuffer->init(vertices);
    vertexBuffer->bind();

    elementBuffer = elementBuffer_;
    // elementBuffer->init(faces);
    elementBuffer->bind();

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                          reinterpret_cast<GLvoid*>(offsetof(Vertex, U)));
    glEnableVertexAttribArray(1);
    glVertexAttribIPointer(
        2, 1, GL_INT, sizeof(Vertex),
        reinterpret_cast<GLvoid*>(offsetof(Vertex, MaterialIndex)));
    glEnableVertexAttribArray(2);

    unbind();
}

void TriangleArray::destroy() {
    if (elementBuffer) {
        // elementBuffer->destroy();
        elementBuffer.reset();
    }

    if (vertexBuffer) {
        // vertexBuffer->destroy();
        vertexBuffer.reset();
    }

    if (isValid()) {
        glDeleteVertexArrays(1, &arrayIndex);
        arrayIndex = INVALID_ARRAY_INDEX;
    }
}

void TriangleArray::bind() const {
    glBindVertexArray(arrayIndex);
}

void TriangleArray::unbind() const {
    glBindVertexArray(INVALID_ARRAY_INDEX);
}

bool TriangleArray::isValid() const {
    return arrayIndex != INVALID_ARRAY_INDEX;
}

size_t TriangleArray::getSize() const {
    if (elementBuffer) {
        return elementBuffer->getSize();
    }
    return 0;
}

void TriangleArray::draw() const {
    if (isValid()) {
        draw(0, getSize());
    }
}

void TriangleArray::draw(size_t firstIndex, size_t count) const {
    if (isValid()) {
        bind();
        glDrawElements( GL_TRIANGLES, count*3, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLuint) * firstIndex * 3) );
        unbind();
    }
}

std::weak_ptr<VertexBuffer> TriangleArray::getVertexBuffer() const {
    return vertexBuffer;
}
