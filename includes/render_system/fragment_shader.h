#pragma once
#include <string>
#include <utils/gl.h>

class FragmentShader {
public:
	void init(const std::string& text);
	void destroy();

	void bind() const;
	void unbind() const;

	bool isValid() const;

	GLuint getIndex() const;
private:
    static constexpr GLuint INVALID_SHADER_INDEX = 0;
	GLuint shaderIndex = INVALID_SHADER_INDEX;
};