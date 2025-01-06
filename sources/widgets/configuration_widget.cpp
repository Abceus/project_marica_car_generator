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

// wxDEFINE_EVENT(COLLISION_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(TIRE_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_STEER_ACROSS_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_STEER_ALONG_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_ENG_ACROSS_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_ENG_ALONG_CHANGED, wxCommandEvent);
// wxDEFINE_EVENT(WHEEL_VERT_CHANGED, wxCommandEvent);
// #ifdef WITH_PHYSICS
// wxDEFINE_EVENT(EMULATE_BUTTON_CLICKED, wxCommandEvent);
// #endif

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
}

void ConfigurationWidget::draw() {
    meshFilePicker.draw();

    collisionFilePicker.draw();

    // Skins
    {
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

        for (int i = oldSize; i < newSize; ++i) {
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