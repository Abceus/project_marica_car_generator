#pragma once
#include <wx/window.h>
#include "widgets/pgproperties/texture_array_pgproperty.h"

wxDECLARE_EVENT(MESH_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(COLLISION_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(TIRE_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(WHEEL_STEER_ACROSS_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(WHEEL_STEER_ALONG_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(WHEEL_ENG_ACROSS_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(WHEEL_ENG_ALONG_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(WHEEL_VERT_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(SKIN_CHANGED, wxCommandEvent);
#ifdef WITH_PHYSICS
wxDECLARE_EVENT(EMULATE_BUTTON_CLICKED, wxCommandEvent);
#endif

class ConfigurationWidget : public wxWindow
{
public:
    ConfigurationWidget(wxWindow* parent);

    void resizeTextureArray(size_t newSize);
    void setTexture(size_t index, const std::string& newPath);

private:
    wxPGProperty* textureArrayProperty = nullptr;
};
