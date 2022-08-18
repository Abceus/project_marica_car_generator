#include "render_system/texture.h"

void Texture::init(const wxImage& image) {
    if (isValid()) {
        destroy();
    }

    glGenTextures(1, &textureIndex);
    activate();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, image.HasAlpha() ? 4 : 3, image.GetWidth(),
                 image.GetHeight(), 0, image.HasAlpha() ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, image.GetData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::destroy() {
    if (isValid()) {
        glDeleteTextures(1, &textureIndex);
        textureIndex = INVALID_TEXTURE_INDEX;
    }
}

void Texture::activate() const {
    glActiveTexture(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureIndex);
}

void Texture::deactivate() const {
    glBindTexture(GL_TEXTURE_2D, INVALID_TEXTURE_INDEX);
}

bool Texture::isValid() const {
    return textureIndex != INVALID_TEXTURE_INDEX;
}