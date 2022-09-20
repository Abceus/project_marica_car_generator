#pragma once
#include <wx/clntdata.h>
#include "utils/math/vec3.h"

template<typename T>
struct Vec3Data final : public wxClientData {
    Vec3<T> value;
};
