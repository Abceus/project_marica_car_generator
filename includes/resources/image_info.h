#pragma once

#include <cstddef>
#include <memory>
#include <optional>
#include <string>
#include <vector>

class ImageInfo {
public:
    ImageInfo() = default;
    ImageInfo(const std::string& path);

    unsigned int getHeight() const;
    unsigned int getWidth() const;
    unsigned int getComponentAmount() const;

    std::vector<std::byte> getData() const;

private:
    int height = 0;
    int width = 0;
    int componentAmount = 0;
    std::vector<std::byte> data;
};