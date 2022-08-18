#pragma once
#include <wx/window.h>

wxDECLARE_EVENT(MESH_CHANGED, wxCommandEvent);
wxDECLARE_EVENT(COLLISION_CHANGED, wxCommandEvent);

class ConfigurationWidget : public wxWindow
{
public:
    ConfigurationWidget(wxWindow* parent);
};
