#pragma once
#include "resources/model.h"
#include "resources/wireframe_model.h"

class ElementBuffer {
public:
    void init(const std::vector<Face>& faces);
    void init(const std::vector<Edge>& edges);
    void destroy();

    void bind() const;
    void unbind() const;

    bool isValid() const;

	size_t getSize() const;
private:
    static constexpr GLuint INVALID_BUFFER_INDEX = 0;
    GLuint bufferIndex = INVALID_BUFFER_INDEX;
	size_t size = 0;
};