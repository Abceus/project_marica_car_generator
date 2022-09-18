#pragma once
#include <wx/clntdata.h>

struct IndexedTexture final : public wxClientData {
    size_t index = 0;
    std::string path;
};