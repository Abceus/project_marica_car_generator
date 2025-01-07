#include "widgets/configuration_widget.h"
#include "imgui.h"
#include <filesystem>
#include <format>
#include <iterator>
#include <limits>

using json = nlohmann::json;

ConfigurationWidget::ConfigurationWidget() {
    meshFilePicker.setFileExtensions(".psk");
    meshFilePicker.setDefaultTitle("Select Mesh");

    collisionFilePicker.setFileExtensions(".psk"); // ase
    collisionFilePicker.setDefaultTitle("Select Collision");

    tireCollisionFilePicker.setFileExtensions(".psk"); // ase
    tireCollisionFilePicker.setDefaultTitle("Select Tire Collision");
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
            auto& addedWidget = skinsWidgets.emplace_back(std::format("SkinFileDlgId %d", i));
            addedWidget.setFileExtensions(".tga,.dds");
            addedWidget.setDefaultTitle("Choose texture");
            addedWidget.getReact().setCallback([this, i](const std::filesystem::path& path) {
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

void ConfigurationWidget::load(const json& json) {
}

json ConfigurationWidget::save() const {
    json result;

    return result;
}