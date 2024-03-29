#pragma once
#include <wx/clntdata.h>
#include <filesystem>

struct IndexedTextureData final : public wxClientData {
    size_t index = 0;
    std::filesystem::path path;
};