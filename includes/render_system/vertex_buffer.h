#pragma once
#include "resources/model.h"

class VertexBuffer {
public:
    void init(const std::vector<Vertex>& vertices);
    void destroy();

    void bind() const;
    void unbind() const;

    bool isValid() const;

private:
    static constexpr GLuint INVALID_BUFFER_INDEX = 0;
    GLuint bufferIndex = INVALID_BUFFER_INDEX;
};