#pragma once

#include "widgets/file_picker_widget.h"
#include "widgets/float_input.h"
#include <filesystem>
#include <functional>
#include <nlohmann/json.hpp>
#include <string>

class ConfigurationWidget {
    using json = nlohmann::json;

public:
    using PathReact = FilePickerWidget::ReactType;
    using SkinChangedCallbackType = std::function<void(size_t, const std::filesystem::path&)>;
    using ButtonClickCallbackType = std::function<void()>;
    using FloatReact = FloatInput::ReactType;

public:
    ConfigurationWidget();

    void draw();
    void resizeTextureArray(size_t newSize);
    void setTexture(size_t index, const std::filesystem::path& newPath);

    PathReact& getMeshReact() {
        return meshFilePicker.getReact();
    }

    PathReact& getCollisionReact() {
        return collisionFilePicker.getReact();
    }

    PathReact& getTireCollisionReact() {
        return tireCollisionFilePicker.getReact();
    }

    void setSkinChangedCallback(const SkinChangedCallbackType& callback) {
        skinChangedCallback = callback;
    }

    void setEmulateButtonPressedCallback(const ButtonClickCallbackType& callback) {
        emulateButtonPressedCallback = callback;
    }

    FloatReact& getWheelSteerAcrossReact() {
        return wheelSteerAcrossInput.getReact();
    }

    FloatReact& getWheelSteerAlongReact() {
        return wheelSteerAlongInput.getReact();
    }

    FloatReact& getWheelEngAcrossReact() {
        return wheelEngAcrossInput.getReact();
    }

    FloatReact& getWheelEngAlongReact() {
        return wheelEngAlongInput.getReact();
    }

    FloatReact& getWheelVertReact() {
        return wheelVertInput.getReact();
    }

    void load(const json& json);
    json save() const;

private:
    SkinChangedCallbackType skinChangedCallback;
    ButtonClickCallbackType emulateButtonPressedCallback;

    FloatInput wheelSteerAcrossInput{"WheelSteerAcrossInput", 0.0f};
    FloatInput wheelSteerAlongInput{"WheelSteerAlongInput", 0.0f};
    FloatInput wheelEngAcrossInput{"WheelEngAcrossInput", 0.0f};
    FloatInput wheelEngAlongInput{"WheelEngAlongInput", 0.0f};
    FloatInput wheelVertInput{"WheelVertInput", 0.0f};

    std::vector<FilePickerWidget> skinsWidgets;

    FilePickerWidget meshFilePicker{"MeshFilePicker"};
    FilePickerWidget collisionFilePicker{"CollisionFilePicker"};
    FilePickerWidget tireCollisionFilePicker{"TireCollisionFilePicker"};
};
