#include "render_system/edge_array.h"
#include "render_system/vertex_buffer.h"
#include <memory>

void EdgeArray::init(const std::shared_ptr<VertexBuffer>& vertexBuffer_,
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

    unbind();
}

void EdgeArray::destroy() {
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

void EdgeArray::bind() const {
    glBindVertexArray(arrayIndex);
}

void EdgeArray::unbind() const {
    glBindVertexArray(INVALID_ARRAY_INDEX);
}

bool EdgeArray::isValid() const {
    return arrayIndex != INVALID_ARRAY_INDEX;
}

size_t EdgeArray::getSize() const {
    if (elementBuffer) {
        return elementBuffer->getSize();
    }
    return 0;
}

void EdgeArray::draw() const {
    if (isValid()) {
        draw(0, getSize());
    }
}

void EdgeArray::draw(size_t firstIndex, size_t count) const {
    if (isValid()) {
        bind();
        glDrawElements( GL_LINES, count*2, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(GLuint) * firstIndex * 2) );
        unbind();
    }
}