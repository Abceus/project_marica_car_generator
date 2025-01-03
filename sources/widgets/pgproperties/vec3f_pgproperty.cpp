#include "widgets/pgproperties/vec3f_pgproperty.h"
#include "widgets/variants/vec3f.h"

// Vec3fPGProperty::Vec3fPGProperty(const wxString& label, const wxString& name,
//                                  const Vec3f& value)
//     : wxPGProperty(label, name) {

//     AddPrivateChild(new wxFloatProperty("X:", "X", value.getX()));
//     AddPrivateChild(new wxFloatProperty("Y:", "Y", value.getY()));
//     AddPrivateChild(new wxFloatProperty("Z:", "Z", value.getZ()));

//     SetValue(wxVariant(new Vec3fVariantData(value)));
// }

// void Vec3fPGProperty::OnSetValue() {
//     auto value =
//         dynamic_cast<Vec3fVariantData*>(GetValue().GetData())->getValue();
//     m_children[0]->SetValue(wxVariant(value.getX()));
//     m_children[1]->SetValue(wxVariant(value.getY()));
//     m_children[2]->SetValue(wxVariant(value.getZ()));
// }

// wxString Vec3fPGProperty::GetValueAsString(int argFlags) const {
//     auto value =
//         dynamic_cast<Vec3fVariantData*>(GetValue().GetData())->getValue();
//     return "(" + std::to_string(value.getX()) + ", " +
//            std::to_string(value.getY()) + ", " + std::to_string(value.getZ())
//            +
//            ")";
// }

// wxVariant Vec3fPGProperty::ChildChanged(wxVariant& thisValue, int childIndex,
//                                         wxVariant& childValue) const {
//     auto value =
//         dynamic_cast<Vec3fVariantData*>(GetValue().GetData())->getValue();
//     if(childIndex == 0) {
//         value.setX(childValue.GetDouble());
//     } else if(childIndex == 1) {
//         value.setY(childValue.GetDouble());
//     } else if(childIndex == 2) {
//         value.setZ(childValue.GetDouble());
//     }
//     return wxVariant(new Vec3fVariantData(value));
// }
