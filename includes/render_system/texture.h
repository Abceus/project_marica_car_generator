#pragma once
#include "resources/dds_info.h"
#include "resources/image_info.h"
#include "utils/gl.h"

class Texture {
public:
    void init(const ImageInfo& image);
    void init(const DDSInfo& ddsInfo);
    void destroy();

    void activate() const;
    void deactivate() const;

    bool isValid() const;

private:
    static constexpr GLuint INVALID_TEXTURE_INDEX = 0;
    GLuint textureIndex = INVALID_TEXTURE_INDEX;
};