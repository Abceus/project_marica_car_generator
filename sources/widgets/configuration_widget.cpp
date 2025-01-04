#include "widgets/configuration_widget.h"
#include "imgui.h"
#include "widgets/event_data/float.h"
#include "widgets/event_data/indexed_texture.h"
#include "widgets/pgproperties/texture_array_pgproperty.h"
#include "widgets/pgproperties/vec3f_pgproperty.h"
#include <ImGuiFileDialog.h>
#include <format>
#include <limits>

// wxDEFINE_EVENT(MESH_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(COLLISION_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(TIRE_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_STEER_ACROSS_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_STEER_ALONG_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_ENG_ACROSS_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_ENG_ALONG_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_VERT_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(SKIN_CHANGED, wxCommandEvent);
#ifdef WITH_PHYSICS
wxDEFINE_EVENT(EMULATE_BUTTON_CLICKED, wxCommandEvent);
#endif

ConfigurationWidget::ConfigurationWidget() {
    // textureArrayProperty = grid->Append(new TextureArrayPGProperty("Skins"));
    // grid->Bind(wxEVT_PG_CHANGED, [this](wxPropertyGridEvent& event) {
    //     if (event.GetMainParent() != textureArrayProperty) {
    //         event.Skip();
    //         return;
    //     }
    //     auto data = new IndexedTextureData();
    //     data->index =
    //     event.GetProperty()->GetAttribute("index").GetInteger(); data->path =
    //     event.GetValue().GetString().ToStdString(); wxCommandEvent
    //     newEvent(SKIN_CHANGED); newEvent.SetClientObject(data);
    //     wxPostEvent(this, newEvent);
    // });

    // // textureArrayWidget = new TextureArrayWidget(this);
    // // sizer->Add(textureArrayWidget, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    // // textureArrayWidget->Bind(TEXTURE_CHANGED,
    // //                  [this](const wxCommandEvent& event) {
    // //                      wxPostEvent(this, event);
    // //                  });

    // auto vec3Property = grid->Append(new Vec3fPGProperty("Coords"));
    // grid->Bind(wxEVT_PG_CHANGED, [vec3Property](wxPropertyGridEvent& event) {
    //     if (event.GetMainParent() != vec3Property) {
    //         event.Skip();
    //         return;
    //     }
    // });

    // auto collisionPicker = new wxFilePickerCtrl(
    //     // this, wxID_ANY, wxEmptyString, "Select collision", "ASE
    //     // (*.ase)|*.ase");
    //     this, wxID_ANY, wxEmptyString, "Select collision", "PSK
    //     (*.psk)|*.psk");
    // collisionPicker->SetWindowStyleFlag(wxFLP_DEFAULT_STYLE | wxFLP_OPEN |
    //                                     wxFLP_FILE_MUST_EXIST);
    // sizer->Add(collisionPicker, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    // collisionPicker->Bind(wxEVT_FILEPICKER_CHANGED,
    //                       [this](const wxFileDirPickerEvent& event) {
    //                           wxCommandEvent
    //                           collisionEvent(COLLISION_CHANGED);
    //                           collisionEvent.SetString(event.GetPath());
    //                           wxPostEvent(this, collisionEvent);
    //                       });

    // auto tirePicker = new wxFilePickerCtrl(
    //     // this, wxID_ANY, wxEmptyString, "Select collision", "ASE
    //     // (*.ase)|*.ase");
    //     this, wxID_ANY, wxEmptyString, "Select tire", "PSK (*.psk)|*.psk");
    // tirePicker->SetWindowStyleFlag(wxFLP_DEFAULT_STYLE | wxFLP_OPEN |
    //                                wxFLP_FILE_MUST_EXIST);
    // sizer->Add(tirePicker, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    // tirePicker->Bind(wxEVT_FILEPICKER_CHANGED,
    //                  [this](const wxFileDirPickerEvent& event) {
    //                      wxCommandEvent collisionEvent(TIRE_CHANGED);
    //                      collisionEvent.SetString(event.GetPath());
    //                      wxPostEvent(this, collisionEvent);
    //                  });

    // auto wheelSteerAcrossCtrl = new wxSpinCtrlDouble(this);
    // sizer->Add(wheelSteerAcrossCtrl, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    // wheelSteerAcrossCtrl->SetRange(std::numeric_limits<float>::lowest(),
    //                                std::numeric_limits<float>::max());
    // wheelSteerAcrossCtrl->SetValue(0.0);
    // wheelSteerAcrossCtrl->Bind(
    //     wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) {
    //         auto data = new FloatData();
    //         data->value = static_cast<float>(event.GetValue());
    //         wxCommandEvent newEvent(WHEEL_STEER_ACROSS_CHANGED);
    //         newEvent.SetClientObject(data);
    //         wxPostEvent(this, newEvent);
    //     });

    // auto wheelSteerAlongCtrl = new wxSpinCtrlDouble(this);
    // sizer->Add(wheelSteerAlongCtrl, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    // wheelSteerAlongCtrl->SetRange(std::numeric_limits<float>::lowest(),
    //                               std::numeric_limits<float>::max());
    // wheelSteerAlongCtrl->SetValue(0.0);
    // wheelSteerAlongCtrl->Bind(
    //     wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) {
    //         auto data = new FloatData();
    //         data->value = static_cast<float>(event.GetValue());
    //         wxCommandEvent newEvent(WHEEL_STEER_ALONG_CHANGED);
    //         newEvent.SetClientObject(data);
    //         wxPostEvent(this, newEvent);
    //     });

    // auto wheelEngAcrossCtrl = new wxSpinCtrlDouble(this);
    // sizer->Add(wheelEngAcrossCtrl, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    // wheelEngAcrossCtrl->SetRange(std::numeric_limits<float>::lowest(),
    //                              std::numeric_limits<float>::max());
    // wheelEngAcrossCtrl->SetValue(0.0);
    // wheelEngAcrossCtrl->Bind(
    //     wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) {
    //         auto data = new FloatData();
    //         data->value = static_cast<float>(event.GetValue());
    //         wxCommandEvent newEvent(WHEEL_ENG_ACROSS_CHANGED);
    //         newEvent.SetClientObject(data);
    //         wxPostEvent(this, newEvent);
    //     });

    // auto wheelEngAlongCtrl = new wxSpinCtrlDouble(this);
    // sizer->Add(wheelEngAlongCtrl, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    // wheelEngAlongCtrl->SetRange(std::numeric_limits<float>::lowest(),
    //                             std::numeric_limits<float>::max());
    // wheelEngAlongCtrl->SetValue(0.0);
    // wheelEngAlongCtrl->Bind(
    //     wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) {
    //         auto data = new FloatData();
    //         data->value = static_cast<float>(event.GetValue());
    //         wxCommandEvent newEvent(WHEEL_ENG_ALONG_CHANGED);
    //         newEvent.SetClientObject(data);
    //         wxPostEvent(this, newEvent);
    //     });

    // auto wheelVertCtrl = new wxSpinCtrlDouble(this);
    // sizer->Add(wheelVertCtrl, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    // wheelVertCtrl->SetRange(std::numeric_limits<float>::lowest(),
    //                         std::numeric_limits<float>::max());
    // wheelVertCtrl->SetValue(0.0);
    // wheelVertCtrl->Bind(wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent&
    // event) {
    //     auto data = new FloatData();
    //     data->value = static_cast<float>(event.GetValue());
    //     wxCommandEvent newEvent(WHEEL_VERT_CHANGED);
    //     newEvent.SetClientObject(data);
    //     wxPostEvent(this, newEvent);
    // });

#ifdef WITH_PHYSICS
    auto emulateButton = new wxButton(this, wxID_ANY);
    emulateButton->SetLabel("Emulate");
    sizer->Add(emulateButton, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);

    emulateButton->Bind(wxEVT_BUTTON, [this](const auto& event) {
        wxCommandEvent collisionEvent(EMULATE_BUTTON_CLICKED);
        wxPostEvent(this, collisionEvent);
    });
#endif
}

void ConfigurationWidget::draw() {
    if (ImGui::Button("Open Mesh")) {
        IGFD::FileDialogConfig config;
        config.path = ".";
        ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey",
                                                "Choose Mesh", ".psk", config);
    }

    if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            std::string filePathName =
                ImGuiFileDialog::Instance()->GetFilePathName();
            if (meshChangedCallback) {
                meshChangedCallback(filePathName);
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }

    ImGui::BeginTable("Skins", 2);

    for (auto i = 0; i < skinsArray.size(); ++i) {
        ImGui::PushID(i);

        ImGui::Text("%d", i);
        ImGui::TableNextColumn();

        const auto fileDialogId = std::format("Skin %d", i);
        if (ImGui::Button(skinsArray[i].string().c_str())) {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog(fileDialogId.c_str(),
                                                    "Choose Skin",
                                                    ".dds,.tga",
                                                    config);
        }

        if (ImGuiFileDialog::Instance()->Display(fileDialogId.c_str())) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                if (skinChangedCallback) {
                    skinChangedCallback(i, filePathName);
                }
            }

            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::TableNextRow();

        ImGui::PopID();
    }

    ImGui::EndTable();
}

void ConfigurationWidget::resizeTextureArray(size_t newSize) {
    skinsArray.resize(newSize);
}

void ConfigurationWidget::setTexture(size_t index, const std::filesystem::path& newPath) {
    if (index >= skinsArray.size()) {
        return;
    }

    skinsArray[index] = newPath;
}

void ConfigurationWidget::setMeshChangedCallback(
    const MeshChangedCallbackType& callback) {
    meshChangedCallback = callback;
}