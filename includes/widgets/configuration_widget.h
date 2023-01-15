#pragma once
#include "mul_widget.h"
#include "widgets/mul_file_picker.h"
#include <string>
#include <mul_widget.h>
#include <filesystem>
#include <mul_callback_owner.h>
// #include <wx/window.h>
// #include "widgets/pgproperties/texture_array_pgproperty.h"

// wxDECLARE_EVENT(MESH_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(COLLISION_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(TIRE_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_STEER_ACROSS_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_STEER_ALONG_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_ENG_ACROSS_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_ENG_ALONG_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(WHEEL_VERT_CHANGED, wxCommandEvent);
// wxDECLARE_EVENT(SKIN_CHANGED, wxCommandEvent);
#ifdef WITH_PHYSICS
//wxDECLARE_EVENT(EMULATE_BUTTON_CLICKED, wxCommandEvent);
#endif

class ConfigurationWidget : public MulWidget
{
public:
    ConfigurationWidget() = default;
    void onInit() override;
    void onDraw() override {}
    void onUpdate(float dt) override {}

    void resizeTextureArray(size_t newSize);
    void setTexture(size_t index, const std::string& newPath);

    using MeshChangedCallbackType = std::shared_ptr<MulCallbackOwner<std::filesystem::path>>;
    void addMeshChangedCallback(MeshChangedCallbackType& outHandler, const std::function<void(std::filesystem::path)>& callback);
private:
    MulFilePicker::ChangePathCallbackType meshModelSelect;
    // wxPGProperty* textureArrayProperty = nullptr;

    MulCallbackCollection<std::filesystem::path> meshChangedCallbackCollection;
};
