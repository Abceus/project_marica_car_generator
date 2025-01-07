#pragma once

#include "widgets/file_picker_widget.h"
#include <filesystem>
#include <functional>
#include <string>

class ConfigurationWidget {
public:
    using MeshChangedCallbackType = std::function<void(const std::filesystem::path&)>;
    using SkinChangedCallbackType = std::function<void(size_t, const std::filesystem::path&)>;
    using ButtonClickCallbackType = std::function<void()>;
    using FloatChangedCallbackType = std::function<void(float)>;

public:
    ConfigurationWidget();

    void draw();
    void resizeTextureArray(size_t newSize);
    void setTexture(size_t index, const std::filesystem::path& newPath);

    void setMeshChangedCallback(const MeshChangedCallbackType& callback);
    void setCollisionChangedCallback(const MeshChangedCallbackType& callback) {
        collisionChangedCallback = callback;
    }
    void setTireCollisionChangedCallback(const MeshChangedCallbackType& callback) {
        tireCollisionChangedCallback = callback;
    }
    void setSkinChangedCallback(const SkinChangedCallbackType& callback) {
        skinChangedCallback = callback;
    }
    void setEmulateButtonPressedCallback(const ButtonClickCallbackType& callback) {
        emulateButtonPressedCallback = callback;
    }
    void setWheelSteerAcrossChangedCallback(const FloatChangedCallbackType& callback) {
        wheelSteerAcrossChangedCallback = callback;
    }
    void setWheelSteerAlongChangedCallback(const FloatChangedCallbackType& callback) {
        wheelSteerAlongChangedCallback = callback;
    }
    void setWheelEngAcrossChangedCallback(const FloatChangedCallbackType& callback) {
        wheelEngAcrossChangedCallback = callback;
    }
    void setWheelEngAlongChangedCallback(const FloatChangedCallbackType& callback) {
        wheelEngAlongChangedCallback = callback;
    }
    void setWheelVertChangedCallback(const FloatChangedCallbackType& callback) {
        wheelVertChangedCallback = callback;
    }

private:
    MeshChangedCallbackType meshChangedCallback;
    MeshChangedCallbackType collisionChangedCallback;
    MeshChangedCallbackType tireCollisionChangedCallback;
    SkinChangedCallbackType skinChangedCallback;
    ButtonClickCallbackType emulateButtonPressedCallback;
    FloatChangedCallbackType wheelSteerAcrossChangedCallback;
    FloatChangedCallbackType wheelSteerAlongChangedCallback;
    FloatChangedCallbackType wheelEngAcrossChangedCallback;
    FloatChangedCallbackType wheelEngAlongChangedCallback;
    FloatChangedCallbackType wheelVertChangedCallback;

    std::vector<FilePickerWidget> skinsWidgets;

    FilePickerWidget meshFilePicker{"MeshFilePicker"};
    FilePickerWidget collisionFilePicker{"CollisionFilePicker"};
    FilePickerWidget tireCollisionFilePicker{"TireCollisionFilePicker"};

    float wheelSteerAcross = 0.0f;
    float wheelSteerAlong = 0.0f;
    float wheelEngAcross = 0.0f;
    float wheelEngAlong = 0.0f;
    float vertAlong = 0.0f;
};
