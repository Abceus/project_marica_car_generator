#include "render_system/color.h"

Color::Color(float red, float green, float blue, float alpha)
    : red(red), green(green), blue(blue), alpha(alpha) {}

float Color::getRed() const {
    return red;
}
void Color::setRed(float value) {
    red = value;
}

float Color::getGreen() const {
    return green;
}
void Color::setGreen(float value) {
    green = value;
}

float Color::getBlue() const {
    return blue;
}
void Color::setBlue(float value) {
    blue = value;
}

float Color::getAlpha() const {
    return alpha;
}
void Color::setAlpha(float value) {
    alpha = value;
}


glm::vec4 Color::getGLVec4() const {
	return {red, green, blue, alpha};
}