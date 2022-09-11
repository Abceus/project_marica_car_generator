#pragma once
#include <wx/window.h>

wxDECLARE_EVENT(MESH_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(COLLISION_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(TIRE_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(EMULATE_BUTTON_CLICKED, wxCommandEvent);

class ConfigurationWidget : public wxWindow
{
public:
    ConfigurationWidget(wxWindow* parent);
};
