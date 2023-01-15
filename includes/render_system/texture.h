#pragma once
#include <mul_opengl.h>
#include <mul_image.h>
#include "resources/dds_info.h"

class Texture {
public:
    void init(const std::shared_ptr<MulImage>& image);
    void init(const DDSInfo& ddsInfo);
    void destroy();

    void activate() const;
    void deactivate() const;

    bool isValid() const;

private:
    static constexpr GLuint INVALID_TEXTURE_INDEX = 0;
    GLuint textureIndex = INVALID_TEXTURE_INDEX;
};