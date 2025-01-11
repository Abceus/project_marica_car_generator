#pragma once

#include "utils/openal.h"

class OpenALContext {
public:
    bool init();
    void deinit();

    ~OpenALContext();

private:
    bool inited = false;
};