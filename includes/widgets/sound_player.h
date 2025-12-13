#pragma once

#include "sound/sound.h"
#include "widgets/react/primitive.h"
#include <memory>
#include <string>

class SoundPlayer {
public:
    using ReactType = ReactPrimitive<std::unique_ptr<Sound>>;

public:
    SoundPlayer() = default;

    SoundPlayer(std::unique_ptr<Sound>&& sound) {
        react.move(std::move(sound));
    }

    void draw();

    ReactType& getReact() {
        return react;
    }

    const ReactType& getReact() const {
        return react;
    }

    void setFormat(const std::string& newFormat) {
        format = newFormat;
    }

private:
    ReactType react;

    bool isPlaying = false;
    std::string format = "{:%T/%T}";
};