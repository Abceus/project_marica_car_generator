#pragma once
#include "utils/math/vec3.h"
#include "widgets/event_data/vec3.h"
#include <wx/sizer.h>
#include <wx/window.h>
#include "wx/spinctrl.h"
#include <wx/stattext.h>

wxDEFINE_EVENT(VALUE_CHANGED, wxCommandEvent);

template <typename T>
class Vec3Widget : public wxWindow {
public:
    Vec3Widget(wxWindow* parent);

    Vec3<T> getValue() const;
    void setValue(const Vec3<T>& newValue);

private:
    Vec3<T> value;

    wxSpinCtrlDouble* spinCtrlX = nullptr;
    wxSpinCtrlDouble* spinCtrlY = nullptr;
    wxSpinCtrlDouble* spinCtrlZ = nullptr;
};

template <typename T>
Vec3Widget<T>::Vec3Widget(wxWindow* parent) : wxWindow(parent, wxID_ANY) {
    auto sizer = new wxGridSizer(4);
    sizer->SetSizeHints(GetParent());
    SetSizer(sizer);

    auto labelX = new wxStaticText(this, wxID_ANY, "X:");
    sizer->Add(labelX, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);

    spinCtrlX = new wxSpinCtrlDouble(this);
    sizer->Add(spinCtrlX, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    spinCtrlX->SetRange(std::numeric_limits<T>::lowest(),
                                   std::numeric_limits<T>::max());
    spinCtrlX->SetValue(0.0);
    spinCtrlX->Bind(
        wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) {
            auto data = new Vec3Data<T>();
            value.setX(static_cast<T>(event.GetValue()));
            data->value = value;
            wxCommandEvent newEvent(VALUE_CHANGED);
            newEvent.SetClientObject(data);
            wxPostEvent(this, newEvent);
        });

    spinCtrlY = new wxSpinCtrlDouble(this);
    sizer->Add(spinCtrlY, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    spinCtrlY->SetRange(std::numeric_limits<T>::lowest(),
                                   std::numeric_limits<T>::max());
    spinCtrlY->SetValue(0.0);
    spinCtrlY->Bind(
        wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) {
            auto data = new Vec3Data<T>();
            value.setY(static_cast<T>(event.GetValue()));
            data->value = value;
            wxCommandEvent newEvent(VALUE_CHANGED);
            newEvent.SetClientObject(data);
            wxPostEvent(this, newEvent);
        });

    spinCtrlZ = new wxSpinCtrlDouble(this);
    sizer->Add(spinCtrlZ, 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
    spinCtrlZ->SetRange(std::numeric_limits<T>::lowest(),
                                   std::numeric_limits<T>::max());
    spinCtrlZ->SetValue(0.0);
    spinCtrlZ->Bind(
        wxEVT_SPINCTRLDOUBLE, [this](wxSpinDoubleEvent& event) {
            auto data = new Vec3Data<T>();
            value.setZ(static_cast<T>(event.GetValue()));
            data->value = value;
            wxCommandEvent newEvent(VALUE_CHANGED);
            newEvent.SetClientObject(data);
            wxPostEvent(this, newEvent);
        });
}

template <typename T>
Vec3<T> Vec3Widget<T>::getValue() const {
    return value;
}

template <typename T>
void Vec3Widget<T>::setValue(const Vec3<T>& newValue) {
    value = newValue;
    auto data = new Vec3Data<T>();
    data->value = value;
    wxCommandEvent newEvent(VALUE_CHANGED);
    newEvent.SetClientObject(data);
    wxPostEvent(this, newEvent);
}
