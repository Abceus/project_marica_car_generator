#pragma once
// #include "widgets/pgproperties/texture_array_pgproperty.h"
// #include <wx/window.h>

// wxDECLARE_EVENT(MESH_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(COLLISION_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(TIRE_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_STEER_ACROSS_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_STEER_ALONG_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_ENG_ACROSS_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_ENG_ALONG_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_VERT_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(SKIN_CHANGED, wxCommandEvent);
// #ifdef WITH_PHYSICS
// wxDECLARE_EVENT(EMULATE_BUTTON_CLICKED, wxCommandEvent);
// #endif

#include "widgets/file_picker_widget.h"
#include <filesystem>
#include <functional>
#include <string>

class ConfigurationWidget {
public:
    using MeshChangedCallbackType = std::function<void(const std::filesystem::path&)>;
    using SkinChangedCallbackType = std::function<void(size_t, const std::filesystem::path&)>;
    using ButtonClickCallbackType = std::function<void()>;

public:
    ConfigurationWidget();

    void draw();

    void resizeTextureArray(size_t newSize);
    void setTexture(size_t index, const std::filesystem::path& newPath);

    void setMeshChangedCallback(const MeshChangedCallbackType& callback);
    void setCollisionChangedCallback(const MeshChangedCallbackType& callback) {
        collisionChangedCallback = callback;
    }
    void setSkinChangedCallback(const SkinChangedCallbackType& callback) {
        skinChangedCallback = callback;
    }
    void setEmulateButtonPressedCallback(const ButtonClickCallbackType& callback) {
        emulateButtonPressed = callback;
    }

private:
    MeshChangedCallbackType meshChangedCallback;
    MeshChangedCallbackType collisionChangedCallback;
    SkinChangedCallbackType skinChangedCallback;
    ButtonClickCallbackType emulateButtonPressed;

    std::vector<FilePickerWidget> skinsWidgets;

    FilePickerWidget meshFilePicker{"MeshFilePicker"};
    FilePickerWidget collisionFilePicker{"CollisionFilePicker"};
};
