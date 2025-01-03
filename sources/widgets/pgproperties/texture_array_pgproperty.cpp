#include "widgets/pgproperties/texture_array_pgproperty.h"
#include "widgets/event_data/indexed_texture.h"
#include <string>

// TextureArrayPGProperty::TextureArrayPGProperty(const wxString& label,
//                                                const wxString& name,
//                                                const wxArrayString& value)
//     : wxPGProperty(label, name) {
//     SetValue(WXVARIANT(value));
// }

// void TextureArrayPGProperty::OnSetValue() {
//     if (auto grid = GetGrid()) {
//         grid->Freeze();
//     }
//     wxArrayString value = m_value;
//     if (GetChildCount() > value.Count()) {
//         while (GetChildCount() != value.Count()) {
//             delete m_children[GetChildCount() - 1];
//             RemoveChild(GetChildCount() - 1);
//         }
//     }
//     for (size_t i = 0; i < GetChildCount(); ++i) {
//         wxString childValue = m_children[i]->GetValue();
//         if (childValue != value[i]) {
//             m_children[i]->SetValue(wxVariant(value[i]));
//         }
//     }
//     if (GetChildCount() < value.Count()) {
//         for (auto i = GetChildCount(); i < value.Count(); ++i) {
//             auto fileProperty =
//                 new wxFileProperty("[" + std::to_string(i) + "]");
//             auto wildcard =
//                 wxVariant(wxString("Supported formats (*.dds, *.tga, *.bmp, "
//                                    "*.pcx)|*.dds;*.tga;*.bmp;*.pcx|DDS "
//                                    "(*.dds)|*.dds|TGA (*.tga)|*.tga|BMP "
//                                    "(*.bmp)|*.bmp|PCX (*.pcx)|*.pcx"));
//             fileProperty->DoSetAttribute("Wildcard", wildcard);
//             auto showFullPath = wxVariant(1);
//             fileProperty->DoSetAttribute("ShowFullPath", showFullPath);
//             auto index = wxVariant(static_cast<int>(i));
//             fileProperty->SetAttribute("index", index);
//             fileProperty->SetValueFromString(value[i]);
//             AppendChild(fileProperty);
//         }
//     }
//     if (auto grid = GetGrid()) {
//         grid->Thaw();
//     }
// }

// wxString TextureArrayPGProperty::GetValueAsString(int argFlags) const {
//     wxArrayString pathes = m_value;
//     return "Size: " + std::to_string(pathes.size());
// }

// wxVariant TextureArrayPGProperty::ChildChanged(wxVariant& thisValue,
//                                                int childIndex,
//                                                wxVariant& childValue) const {
//     wxArrayString value = m_value;
//     value[childIndex] = childValue.GetString();
//     return wxVariant(value);
// }
