#include "widgets/pgproperties/texture_array_pgproperty.h"
#include "widgets/event_data/indexed_texture.h"
#include "wx/arrstr.h"
#include "wx/msw/window.h"
#include "wx/propgrid/propgriddefs.h"
#include "wx/string.h"
#include "wx/variant.h"
#include <string>
#include <wx/propgrid/propgrid.h>

TextureArrayPGProperty::TextureArrayPGProperty(const wxString& label,
                                               const wxString& name,
                                               const wxArrayString& value)
    : wxPGProperty(label, name) {
    SetValue(WXVARIANT(value));
}

void TextureArrayPGProperty::OnSetValue() {
    if (auto grid = GetGrid()) {
        grid->Freeze();
    }
    wxArrayString value = m_value;
    if (GetChildCount() > value.Count()) {
        while (GetChildCount() != value.Count()) {
            delete m_children[GetChildCount() - 1];
            RemoveChild(GetChildCount() - 1);
        }
    }
    for (size_t i = 0; i < GetChildCount(); ++i) {
        wxString childValue = m_children[i]->GetValue();
        if (childValue != value[i]) {
            m_children[i]->SetValue(wxVariant(value[i]));
        }
    }
    if (GetChildCount() < value.Count()) {
        for (auto i = GetChildCount(); i < value.Count(); ++i) {
            auto fileProperty =
                new wxFileProperty("[" + std::to_string(i) + "]");
            auto wildcard =
                wxVariant(wxString("Supported formats (*.dds, *.tga, *.bmp, "
                                   "*.pcx)|*.dds;*.tga;*.bmp;*.pcx|DDS "
                                   "(*.dds)|*.dds|TGA (*.tga)|*.tga|BMP "
                                   "(*.bmp)|*.bmp|PCX (*.pcx)|*.pcx"));
            fileProperty->DoSetAttribute("Wildcard", wildcard);
            auto showFullPath = wxVariant(1);
            fileProperty->DoSetAttribute("ShowFullPath", showFullPath);
            auto index = wxVariant(static_cast<int>(i));
            fileProperty->SetAttribute("index", index);
            fileProperty->SetValueFromString(value[i]);
            AppendChild(fileProperty);
        }
    }
    if (auto grid = GetGrid()) {
        grid->Thaw();
    }
}

wxString TextureArrayPGProperty::GetValueAsString(int argFlags) const {
    wxArrayString pathes = m_value;
    return "Size: " + std::to_string(pathes.size());
}

wxVariant TextureArrayPGProperty::ChildChanged(wxVariant& thisValue,
                                               int childIndex,
                                               wxVariant& childValue) const {
    wxArrayString value = m_value;
    value[childIndex] = childValue.GetString();
    return wxVariant(value);
}

// void TextureArrayWidget::resize(size_t newSize) {
//     if (pickers.size() > newSize) {
//         for (size_t i = newSize; i < pickers.size(); ++i) {
//             setPath(i, "");
//             pickers[i]->Destroy();
//         }
//     }

//     pickers.resize(newSize);
//     if (newSize > pathes.size()) {
//         for (size_t i = pathes.size(); i < newSize; ++i) {
//             pickers[i] =
//                 new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString,
//                                      "Select texture", "DDS (*.dds)|*.dds|TGA
//                                      (*.tga)|*.tga|BMP (*.bmp)|*.bmp|PCX
//                                      (*.pcx)|*.pcx");

//             pickers[i]->SetWindowStyleFlag(wxFLP_DEFAULT_STYLE | wxFLP_OPEN |
//                                            wxFLP_FILE_MUST_EXIST);
//             sizer->Add(pickers[i], 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
//             pickers[i]->Bind(wxEVT_FILEPICKER_CHANGED,
//                              [this, i](const wxFileDirPickerEvent& event) {
//                                  setPath(i, event.GetPath());
//                              });
//         }
//     }
//     if (auto parent = GetParent()) {
//         if (auto parentSizer = parent->GetSizer()) {
//             parentSizer->Layout();
//         }
//     }
//     pathes.resize(newSize);
// }

// size_t TextureArrayWidget::getSize() const {
//     return pickers.size();
// }

// std::string TextureArrayWidget::getPath(size_t index) const {
//     return pathes[index];
// }

// void TextureArrayWidget::setPath(size_t index, const std::string& newPath) {
//     pathes[index] = newPath;
//     pickers[index]->SetPath(newPath);
//     auto data = new IndexedTextureData();
//     data->index = index;
//     data->path = newPath;
//     wxCommandEvent newEvent(TEXTURE_CHANGED);
//     newEvent.SetClientObject(data);
//     wxPostEvent(this, newEvent);
// }