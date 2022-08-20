#pragma once

#include "glm/glm.hpp"

class Color {
public:
	Color(float red = 0.0f, float green = 0.0f, float blue = 0.0f, float alpha = 1.0f);

	float getRed() const;
	void setRed(float value);

	float getGreen() const;
	void setGreen(float value);
	
	float getBlue() const;
	void setBlue(float value);
	
	float getAlpha() const;
	void setAlpha(float value);

	glm::vec4 getGLVec4() const;
private:
	float red = 0.0f;
	float green = 0.0f;
	float blue = 0.0f;
	float alpha = 1.0f;
};