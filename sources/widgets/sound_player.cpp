#include "widgets/sound_player.h"
#include "imgui.h"

void SoundPlayer::draw() {
    if (!react.getValue()) {
        return;
    }

    if (react.getValue()->isPlaying()) {
        if (ImGui::Button("Pause")) {
            react.getValue()->pause();
        }
    } else {
        if (ImGui::Button("Play")) {
            react.getValue()->play();
        }
    }

    ImGui::SameLine();

    if (ImGui::Button("Stop")) {
        react.getValue()->stop();
    }

    ImGui::SameLine();

    const auto value = react.getValue()->getTime();
    int valueInt = value.count();
    const auto duration = react.getValue()->getDuration();
    int durationInt = duration.count();

    std::string result = std::vformat(format.c_str(), std::make_format_args(value, duration));
    if (ImGui::SliderInt("Timeline", &valueInt, 0, durationInt, result.c_str())) {
        react.getValue()->setTime(std::chrono::milliseconds{valueInt});
    }
}