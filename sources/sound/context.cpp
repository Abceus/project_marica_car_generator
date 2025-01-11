#include "sound/context.h"

#include <iostream>

bool OpenALContext::init() {
    auto device = alcOpenDevice(nullptr);
    if (device == nullptr) {
        std::cout << "Could not open a device!" << std::endl;
        return false;
    }

    auto ctx = alcCreateContext(device, nullptr);
    if (ctx == nullptr || alcMakeContextCurrent(ctx) == ALC_FALSE) {
        if (ctx != nullptr) {
            alcDestroyContext(ctx);
        }
        alcCloseDevice(device);
        std::cout << "Could not set a context!" << std::endl;
        return false;
    }

    inited = true;
    return true;
}

void OpenALContext::deinit() {
    inited = false;

    auto ctx = alcGetCurrentContext();
    if (ctx == nullptr)
        return;

    auto device = alcGetContextsDevice(ctx);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(ctx);
    alcCloseDevice(device);
}

OpenALContext::~OpenALContext() {
    if (inited) {
        deinit();
    }
}