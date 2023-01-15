#include "render_system/texture.h"
#include "resources/dds_info.h"
#include <algorithm>

void Texture::init(const std::shared_ptr<MulImage>& image) {
    if (isValid()) {
        destroy();
    }

    glGenTextures(1, &textureIndex);
    activate();
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    glTexImage2D(GL_TEXTURE_2D, 0, image->getComponents(), image->getWidth(),
                 image->getHeight(), 0, image->getComponents() >= 4 ? GL_RGBA : GL_RGB,
                 GL_UNSIGNED_BYTE, image->getData());

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

void Texture::init(const DDSInfo& ddsInfo) {
    if (isValid()) {
        destroy();
    }

    glGenTextures(1, &textureIndex);
    activate();

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, ddsInfo.getMipMapCount()-1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_NEAREST);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    auto mipMapHeight = ddsInfo.getHeight();
    auto mipMapWidth = ddsInfo.getWidth();
    size_t mipMapOffset = 0;
    int blockSize = 0;
    int format = 0;
    switch (ddsInfo.getCompressType()) {
    case CompressType::DXT1: {
        blockSize = 8;
        format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
        break;
    }
    case CompressType::DXT3: {
        blockSize = 16;
        format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
        break;
    }
    case CompressType::DXT5: {
        blockSize = 16;
        format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
        break;
    }
    default: {
        assert(false);
        break;
    }
    }

    for(size_t i = 0; i < ddsInfo.getMipMapCount(); ++i) {
        auto mipMapSize = std::max(1u, (mipMapWidth+3)/4) * std::max(1u, (mipMapHeight+3)/4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, i, format, mipMapWidth, mipMapHeight, 0, mipMapSize, ddsInfo.getData().data() + mipMapOffset);
        mipMapOffset += mipMapSize;
        mipMapHeight /= 2;
        mipMapWidth /= 2;
    }
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