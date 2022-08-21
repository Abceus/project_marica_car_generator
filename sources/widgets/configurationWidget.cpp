#include "widgets/configurationWidget.h"
#include "wx/filepicker.h"
#include "wx/log.h"
#include "wx/msw/button.h"

wxDEFINE_EVENT(MESH_CHANGED, wxCommandEvent);
wxDEFINE_EVENT(COLLISION_CHANGED, wxCommandEvent);
wxDEFINE_EVENT(EMULATE_BUTTON_CLICKED, wxCommandEvent);

ConfigurationWidget::ConfigurationWidget(wxWindow* parent)
    : wxWindow(parent, wxID_ANY) {
    auto sizer = new wxBoxSizer(wxVERTICAL);
    sizer->SetSizeHints(this);
    SetSizer(sizer);

    auto meshPicker = new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString,
                                           "Select mesh", "PSK (*.psk)|*.psk");
    meshPicker->SetWindowStyleFlag(wxFLP_DEFAULT_STYLE | wxFLP_OPEN |
                                   wxFLP_FILE_MUST_EXIST);
    sizer->Add(meshPicker, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    meshPicker->Bind(wxEVT_FILEPICKER_CHANGED,
                     [this](const wxFileDirPickerEvent& event) {
                         wxCommandEvent meshEvent(MESH_CHANGED);
                         meshEvent.SetString(event.GetPath());
                         wxPostEvent(this, meshEvent);
                     });

    auto collisionPicker = new wxFilePickerCtrl(
        this, wxID_ANY, wxEmptyString, "Select collision", "ASE (*.ase)|*.ase");
    collisionPicker->SetWindowStyleFlag(wxFLP_DEFAULT_STYLE | wxFLP_OPEN |
                                        wxFLP_FILE_MUST_EXIST);
    sizer->Add(collisionPicker, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    collisionPicker->Bind(wxEVT_FILEPICKER_CHANGED,
                          [this](const wxFileDirPickerEvent& event) {
                              wxCommandEvent collisionEvent(COLLISION_CHANGED);
                              collisionEvent.SetString(event.GetPath());
                              wxPostEvent(this, collisionEvent);
                          });

    auto emulateButton = new wxButton(this, wxID_ANY);
    emulateButton->SetLabel("Emulate");
    sizer->Add(emulateButton, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);

    emulateButton->Bind(wxEVT_BUTTON, [this](const auto& event) {
        wxCommandEvent collisionEvent(EMULATE_BUTTON_CLICKED);
        wxPostEvent(this, collisionEvent);
    });
}