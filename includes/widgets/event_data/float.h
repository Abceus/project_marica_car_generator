#pragma once
#include <wx/clntdata.h>

struct FloatData final : public wxClientData {
    float value = 0.0f;
};