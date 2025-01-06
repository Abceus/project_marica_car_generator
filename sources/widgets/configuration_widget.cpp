#include "widgets/configuration_widget.h"
#include "imgui.h"
#include "widgets/event_data/float.h"
#include "widgets/event_data/indexed_texture.h"
#include "widgets/pgproperties/texture_array_pgproperty.h"
#include "widgets/pgproperties/vec3f_pgproperty.h"
#include <filesystem>
#include <format>
#include <iterator>
#include <limits>

ConfigurationWidget::ConfigurationWidget() {
    meshFilePicker.setFileExtensions(".psk");
    meshFilePicker.setDefaultTitle("Select Mesh");
    meshFilePicker.setFilePickedCallback([this](const std::filesystem::path& filePath) {
        if (meshChangedCallback) {
            meshChangedCallback(filePath);
        }
    });

    collisionFilePicker.setFileExtensions(".psk"); // ase
    collisionFilePicker.setDefaultTitle("Select Collision");
    collisionFilePicker.setFilePickedCallback([this](const std::filesystem::path& filePath) {
        if (collisionChangedCallback) {
            collisionChangedCallback(filePath);
        }
    });

    tireCollisionFilePicker.setFileExtensions(".psk"); // ase
    tireCollisionFilePicker.setDefaultTitle("Select Tire Collision");
    tireCollisionFilePicker.setFilePickedCallback([this](const std::filesystem::path& filePath) {
        if (tireCollisionChangedCallback) {
            tireCollisionChangedCallback(filePath);
        }
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

    if (ImGui::InputFloat("Wheel Steer Across", &wheelSteerAcross, 0.01f, 1.0f)) {
        if (wheelSteerAcrossChangedCallback) {
            wheelSteerAcrossChangedCallback(wheelSteerAcross);
        }
    }

    if (ImGui::InputFloat("Wheel Steer Along", &wheelSteerAlong, 0.01f, 1.0f)) {
        if (wheelSteerAlongChangedCallback) {
            wheelSteerAlongChangedCallback(wheelSteerAlong);
        }
    }

    if (ImGui::InputFloat("Wheel Eng Across", &wheelEngAcross, 0.01f, 1.0f)) {
        if (wheelEngAcrossChangedCallback) {
            wheelEngAcrossChangedCallback(wheelEngAcross);
        }
    }

    if (ImGui::InputFloat("Wheel Eng Along", &wheelEngAlong, 0.01f, 1.0f)) {
        if (wheelEngAlongChangedCallback) {
            wheelEngAlongChangedCallback(wheelEngAlong);
        }
    }

    if (ImGui::InputFloat("Vert Along", &vertAlong, 0.01f, 1.0f)) {
        if (wheelVertChangedCallback) {
            wheelVertChangedCallback(vertAlong);
        }
    }

    // Emulate button
    {
#ifdef WITH_PHYSICS
        if (ImGui::Button("Emulate")) {
            if (emulateButtonPressed) {
                emulateButtonPressed();
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
            addedWidget.setFilePickedCallback([this, i](const std::filesystem::path& filePath) {
                if (skinChangedCallback) {
                    skinChangedCallback(i, filePath);
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

    skinsWidgets[index].setPickedPath(newPath);
}

void ConfigurationWidget::setMeshChangedCallback(
    const MeshChangedCallbackType& callback) {
    meshChangedCallback = callback;
}