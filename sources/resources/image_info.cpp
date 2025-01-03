#include "resources/image_info.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <array>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <ios>
#include <memory>
#include <vcruntime_string.h>
#include <vector>

ImageInfo::ImageInfo(const std::string& path) {
    unsigned char* image =
        stbi_load(path.c_str(), &width, &height, &componentAmount, 3);
    const auto dataSize = getHeight() * getWidth() * getComponentAmount();
    data.resize(dataSize);
    memcpy(data.data(), image, dataSize);
    stbi_image_free(image);
}

unsigned int ImageInfo::getHeight() const {
    return height;
}

unsigned int ImageInfo::getWidth() const {
    return width;
}

unsigned int ImageInfo::getComponentAmount() const {
    return componentAmount;
}

std::vector<std::byte> ImageInfo::getData() const {
    return data;
}
