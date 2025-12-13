#pragma once

#include "utils/openal.h"
#include <filesystem>

class Sound {
public:
    Sound(const std::filesystem::path& path);
    ~Sound();

    void play() const;
    void pause() const;
    void stop() const;

    bool isPlaying() const;

    std::chrono::milliseconds getDuration() const;
    void setTime(const std::chrono::milliseconds& milliseconds);
    std::chrono::milliseconds getTime() const;

private:
    ALuint bufferId = 0;
    ALuint sourceId = 0;
};