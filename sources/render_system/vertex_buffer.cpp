#include "render_system/vertex_buffer.h"

void VertexBuffer::init(const std::vector<Vertex>& vertices) {
    if (isValid()) {
        destroy();
    }

    glGenBuffers(1, &bufferIndex);

    bind();

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(),
                 vertices.data(), GL_DYNAMIC_DRAW);

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

void VertexBuffer::updateBuffer(size_t offset,
                                const std::vector<Vertex>& vertices) {
    if (isValid()) {
        bind();
        glBufferSubData(GL_ARRAY_BUFFER, static_cast<GLintptr>(offset) * sizeof(Vertex),
                        static_cast<GLsizeiptr>(vertices.size()) * sizeof(Vertex),
                        reinterpret_cast<const void*>(vertices.data()));
        unbind();
    }
}