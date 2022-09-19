#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <optional>
#include <vector>

enum class CompressType {
    INVALID,
    DXT1,
    DXT3,
    DXT5
};

class DDSInfo {
public:
    static DDSInfo loadDDS(const std::string& path);

    unsigned int getHeight() const;
    unsigned int getWidth() const;
    unsigned int getMipMapCount() const;
    CompressType getCompressType() const;

    std::vector<std::byte> getData() const;
private:
    int height = 0;
    int width = 0;
    int mipMapCount = 0;
    CompressType compressType = CompressType::INVALID;
    std::vector<std::byte> data;
};