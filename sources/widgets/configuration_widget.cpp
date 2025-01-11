#include "widgets/configuration_widget.h"
#include "imgui.h"
#include "sound/sound.h"
#include "widgets/sound_player.h"
#include <filesystem>
#include <format>
#include <iterator>
#include <limits>
#include <memory>

using json = nlohmann::json;

ConfigurationWidget::ConfigurationWidget() {
    meshFilePicker.setFileExtensions(".psk");
    meshFilePicker.setDefaultTitle("Select Mesh");

    collisionFilePicker.setFileExtensions(".psk"); // ase
    collisionFilePicker.setDefaultTitle("Select Collision");

    tireCollisionFilePicker.setFileExtensions(".psk"); // ase
    tireCollisionFilePicker.setDefaultTitle("Select Tire Collision");

    engineSoundFilePicker.setFileExtensions(".wav");
    engineSoundFilePicker.setDefaultTitle("Select Engine Sound");
    engineSoundFilePicker.getReact().getCallbacks().Add([this](const std::filesystem::path& path) {
        engineSoundPlayer.getReact().move(std::make_unique<Sound>(engineSoundFilePicker.getReact().getValue()));
    });
}

void ConfigurationWidget::draw() {
    meshFilePicker.draw();

    // Skins
    {
        if (ImGui::CollapsingHeader("Skins")) {
            ImGui::BeginTable("Skins", 2);

            for (auto i = 0; i < skinsWidgets.size(); ++i) {
                ImGui::PushID(i);

                ImGui::TableNextColumn();
                ImGui::Text("%d", i);

                ImGui::TableNextColumn();
                skinsWidgets[i].draw();

                ImGui::PopID();
            }

            ImGui::EndTable();
        }
    }

    collisionFilePicker.draw();
    tireCollisionFilePicker.draw();

    wheelSteerAcrossInput.draw();
    wheelSteerAlongInput.draw();
    wheelEngAcrossInput.draw();
    wheelEngAlongInput.draw();
    wheelVertInput.draw();

    engineSoundFilePicker.draw();
    engineSoundPlayer.draw();

    // Emulate button
    {
#ifdef WITH_PHYSICS
        if (ImGui::Button("Emulate")) {
            if (emulateButtonPressedCallback) {
                emulateButtonPressedCallback();
            }
        }
#endif
    }
}

void ConfigurationWidget::resizeTextureArray(size_t newSize) {
    if (newSize > skinsWidgets.size()) {
        const auto oldSize = skinsWidgets.size();
        skinsWidgets.reserve(oldSize);

        for (auto i = oldSize; i < newSize; ++i) {
            auto& addedWidget = skinsWidgets.emplace_back(std::format("SkinFileDlgId {}", i));
            addedWidget.setFileExtensions(".tga,.dds");
            addedWidget.setDefaultTitle("Choose texture");
            addedWidget.getReact().getCallbacks().Add([this, i](const std::filesystem::path& path) {
                if (skinChangedCallback) {
                    skinChangedCallback(i, path);
                }
            });
        }
    } else {
        skinsWidgets.erase(std::next(std::begin(skinsWidgets), newSize), std::end(skinsWidgets));
    }
}

void ConfigurationWidget::setTexture(size_t index, const std::filesystem::path& newPath) {
    if (index >= skinsWidgets.size()) {
        return;
    }

    skinsWidgets[index].getReact() = newPath;
}

constexpr auto MESH_PATH_KEY = "meshPath";
constexpr auto COLLISION_PATH_KEY = "collisionPath";
constexpr auto TIRE_COLLISION_PATH_KEY = "tireCollisionPath";
constexpr auto SKINS_PATH_KEY = "skinsPath";
constexpr auto ENGINE_SOUND_PATH_KEY = "engineSoundPath";

constexpr auto WHEEL_STEER_ACROSS_KEY = "wheelSteerAcross";
constexpr auto WHEEL_STEER_ALONG_KEY = "wheelSteerAlong";
constexpr auto WHEEL_ENG_ACROSS_KEY = "wheelEngAcross";
constexpr auto WHEEL_ENG_ALONG_KEY = "wheelEngAlong";
constexpr auto VERT_KEY = "wheelVert";

void ConfigurationWidget::fromJson(const json& json) {
    if (json.contains(MESH_PATH_KEY)) {
        meshFilePicker.getReact() = std::filesystem::path{json.at(MESH_PATH_KEY).get<std::string>()};
    }

    if (json.contains(COLLISION_PATH_KEY)) {
        collisionFilePicker.getReact() = std::filesystem::path{json.at(COLLISION_PATH_KEY).get<std::string>()};
    }

    if (json.contains(TIRE_COLLISION_PATH_KEY)) {
        tireCollisionFilePicker.getReact() = std::filesystem::path{json.at(TIRE_COLLISION_PATH_KEY).get<std::string>()};
    }

    if (json.contains(ENGINE_SOUND_PATH_KEY)) {
        engineSoundFilePicker.getReact() = std::filesystem::path{json.at(ENGINE_SOUND_PATH_KEY).get<std::string>()};
    }

    if (json.contains(SKINS_PATH_KEY)) {
        const auto skinsArray = json.at(SKINS_PATH_KEY);
        resizeTextureArray(skinsArray.size());
        for (auto i = 0; i < skinsArray.size(); ++i) {
            setTexture(i, std::filesystem::path{skinsArray[i].get<std::string>()});
        }
    }

    if (json.contains(WHEEL_STEER_ACROSS_KEY)) {
        wheelSteerAcrossInput.getReact() = json.at(WHEEL_STEER_ACROSS_KEY).get<float>();
    }

    if (json.contains(WHEEL_STEER_ALONG_KEY)) {
        wheelSteerAlongInput.getReact() = json.at(WHEEL_STEER_ALONG_KEY).get<float>();
    }

    if (json.contains(WHEEL_ENG_ACROSS_KEY)) {
        wheelEngAcrossInput.getReact() = json.at(WHEEL_ENG_ACROSS_KEY).get<float>();
    }

    if (json.contains(WHEEL_ENG_ALONG_KEY)) {
        wheelEngAlongInput.getReact() = json.at(WHEEL_ENG_ALONG_KEY).get<float>();
    }

    if (json.contains(VERT_KEY)) {
        wheelVertInput.getReact() = json.at(VERT_KEY).get<float>();
    }
}

json ConfigurationWidget::toJson() const {
    auto result = json::object();

    result[MESH_PATH_KEY] = meshFilePicker.getReact().getValue();
    result[COLLISION_PATH_KEY] = collisionFilePicker.getReact().getValue();
    result[TIRE_COLLISION_PATH_KEY] = tireCollisionFilePicker.getReact().getValue();
    result[ENGINE_SOUND_PATH_KEY] = engineSoundFilePicker.getReact().getValue();

    auto skinsArray = json::array();
    for (const auto& skinWidget : skinsWidgets) {
        skinsArray.emplace_back(skinWidget.getReact().getValue().string());
    }
    result[SKINS_PATH_KEY] = skinsArray;

    result[WHEEL_STEER_ACROSS_KEY] = wheelSteerAcrossInput.getReact().getValue();
    result[WHEEL_STEER_ALONG_KEY] = wheelSteerAlongInput.getReact().getValue();
    result[WHEEL_ENG_ACROSS_KEY] = wheelEngAcrossInput.getReact().getValue();
    result[WHEEL_ENG_ALONG_KEY] = wheelEngAlongInput.getReact().getValue();
    result[VERT_KEY] = wheelVertInput.getReact().getValue();

    return result;
}