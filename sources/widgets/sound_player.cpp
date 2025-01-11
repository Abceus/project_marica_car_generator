#include "widgets/sound_player.h"
#include "imgui.h"

void SoundPlayer::draw() {
    if (!react.getValue()) {
        return;
    }

    if (react.getValue()->isPlaying()) {
        if (ImGui::Button("Pause")) {
            react.getValue()->play();
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

    int value = react.getValue()->getTime().count();
    const auto duration = react.getValue()->getDuration().count();
    if (ImGui::SliderInt("Timeline", &value, 0, duration)) {
        react.getValue()->setTime(std::chrono::seconds{value});
    }

    ImGui::SameLine();

    ImGui::LabelText("Time", "%d:%d", value, duration);
}