#pragma once

#include "sound/sound.h"
#include "widgets/react/primitive.h"
#include <filesystem>
#include <memory>

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

private:
    ReactType react;

    bool isPlaying = false;
};