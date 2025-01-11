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

    std::chrono::seconds getDuration() const;
    void setTime(const std::chrono::seconds& seconds);
    std::chrono::seconds getTime() const;

private:
    ALuint bufferId = 0;
    ALuint sourceId = 0;
};