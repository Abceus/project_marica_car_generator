#include "render_system/element_buffer.h"

void ElementBuffer::init(const std::vector<Face>& faces) {
    if (isValid()) {
        destroy();
    }

    glGenBuffers(1, &bufferIndex);
    size = faces.size();
    bind();

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * faces.size(),
                 faces.data(), GL_STATIC_DRAW);

    unbind();
}

void ElementBuffer::destroy() {
    if (isValid()) {
        glDeleteBuffers(1, &bufferIndex);
        bufferIndex = INVALID_BUFFER_INDEX;
        size = 0;
    }
}

void ElementBuffer::bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferIndex);
}

void ElementBuffer::unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, INVALID_BUFFER_INDEX);
}

bool ElementBuffer::isValid() const {
    return bufferIndex != INVALID_BUFFER_INDEX;
}

size_t ElementBuffer::getSize() const {
    return size;
}