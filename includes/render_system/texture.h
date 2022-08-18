#pragma once
#include "utils/gl.h"
#include <wx/image.h>

class Texture {
public:
    void init(const wxImage& image);
    void destroy();

    void activate() const;
    void deactivate() const;

    bool isValid() const;

private:
    static constexpr GLuint INVALID_TEXTURE_INDEX = 0;
    GLuint textureIndex = INVALID_TEXTURE_INDEX;
};