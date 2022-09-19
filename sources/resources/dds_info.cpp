#include "resources/dds_info.h"
#include <array>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <ios>
#include <memory>
#include <vector>

enum class PixelFlag : unsigned int {
    INVALID = 0x0,
    DDPF_ALPHAPIXELS = 0x1,
    DDPF_ALPHA = 0x2,
    DDPF_FOURCC = 0x4,
    DDPF_RGB = 0x40,
    DDPF_YUV = 0x200,
    DDPF_LUMINANCE = 0x20000
};

struct DDS_PIXELFORMAT {
    using ColorMaskType = std::array<std::byte, 4>;

    unsigned int dwSize;
    PixelFlag dwFlags = PixelFlag::INVALID;
    char dwFourCC[4];
    unsigned int dwRGBBitCount;
    ColorMaskType dwRBitMask;
    ColorMaskType dwGBitMask;
    ColorMaskType dwBBitMask;
    ColorMaskType dwABitMask;
};

enum class DataFlag : unsigned int {
    INVALID = 0x0,
    DDSD_CAPS = 0x1,
    DDSD_HEIGHT = 0x2,
    DDSD_WIDTH = 0x4,
    DDSD_PITCH = 0x8,
    DDSD_PIXELFORMAT = 0x1000,
    DDSD_MIPMAPCOUNT = 0x20000,
    DDSD_LINEARSIZE = 0x80000,
    DDSD_DEPTH = 0x800000,
};

enum class Caps1Flag : unsigned int {
    INVALID = 0x0,
    DDSCAPS_COMPLEX = 0x8,
    DDSCAPS_MIPMAP = 0x400000,
    DDSCAPS_TEXTURE = 0x1000
};

enum class Caps2Flag : unsigned int {
    INVALID = 0x0,
    DDSCAPS2_CUBEMAP = 0x200,
    DDSCAPS2_CUBEMAP_POSITIVEX = 0x400,
    DDSCAPS2_CUBEMAP_NEGATIVEX = 0x800,
    DDSCAPS2_CUBEMAP_POSITIVEY = 0x1000,
    DDSCAPS2_CUBEMAP_NEGATIVEY = 0x2000,
    DDSCAPS2_CUBEMAP_POSITIVEZ = 0x4000,
    DDSCAPS2_CUBEMAP_NEGATIVEZ = 0x8000,
    DDSCAPS2_VOLUME = 0x200000
};

struct DDS_HEADER {
    unsigned int dwSize = 0;
    DataFlag dwFlags = DataFlag::INVALID;
    unsigned int dwHeight = 0;
    unsigned int dwWidth = 0;
    unsigned int dwPitchOrLinearSize = 0;
    unsigned int dwDepth = 0;
    unsigned int dwMipMapCount = 0;
    unsigned int dwReserved1[11];
    DDS_PIXELFORMAT ddspf;
    Caps1Flag dwCaps = Caps1Flag::INVALID;
    Caps2Flag dwCaps2 = Caps2Flag::INVALID;
    unsigned int dwCaps3;
    unsigned int dwCaps4;
    unsigned int dwReserved2;
};

DDSInfo DDSInfo::loadDDS(const std::string& path) {
    DDSInfo result;

    std::ifstream file(path, std::ios_base::ate | std::ios_base::in |
                                 std::ios_base::binary);

    size_t fileSize = file.tellg();
    file.seekg(0, std::ios_base::beg);

    std::string magic;
    magic.resize(4);
    file.read(reinterpret_cast<char*>(magic.data()), magic.size());

    if (magic == "DDS ") {
        DDS_HEADER header;
        file.read(reinterpret_cast<char*>(&header), sizeof(header));

        result.height = header.dwHeight;
        result.width = header.dwWidth;
        result.mipMapCount = header.dwMipMapCount;
        if (std::strncmp(header.ddspf.dwFourCC, "DXT1", 4) == 0) {
            result.compressType = CompressType::DXT1;
        } else if (std::strncmp(header.ddspf.dwFourCC, "DXT3", 4) == 0) {
            result.compressType = CompressType::DXT3;
        } else if (std::strncmp(header.ddspf.dwFourCC, "DXT5", 4) == 0) {
            result.compressType = CompressType::DXT5;
        }

        auto dataSize = fileSize - magic.size() - sizeof(header);
        result.data.resize(dataSize);
        file.read(reinterpret_cast<char*>(result.data.data()), dataSize);
    }

    file.close();
    return result;
}

unsigned int DDSInfo::getHeight() const {
    return height;
}

unsigned int DDSInfo::getWidth() const {
    return width;
}

unsigned int DDSInfo::getMipMapCount() const {
    return mipMapCount;
}

CompressType DDSInfo::getCompressType() const {
    return compressType;
}

std::vector<std::byte> DDSInfo::getData() const {
    return data;
}
