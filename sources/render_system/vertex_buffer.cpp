#include "render_system/vertex_buffer.h"

void VertexBuffer::init(const std::vector<Vertex>& vertices) {
    if (isValid()) {
        destroy();
    }

    glGenBuffers(1, &bufferIndex);

    bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
                 vertices.data(), GL_STATIC_DRAW);

    unbind();
}

void VertexBuffer::destroy() {
    if (isValid()) {
        glDeleteBuffers(1, &bufferIndex);
        bufferIndex = INVALID_BUFFER_INDEX;
    }
}

void VertexBuffer::bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, bufferIndex);
}

void VertexBuffer::unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, INVALID_BUFFER_INDEX);
}

bool VertexBuffer::isValid() const {
    return bufferIndex != INVALID_BUFFER_INDEX;
}