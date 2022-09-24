#pragma once
#include "utils/math/vec3.h"
#include <wx/variant.h>

class Vec3fVariantData : public wxVariantData {
public:
    Vec3fVariantData(const Vec3f& newData = Vec3f());
    Vec3fVariantData(const Vec3fVariantData& other);

    wxVariantData* Clone() const override;
    bool Eq(wxVariantData& data) const override;
    wxString GetType() const override;
    bool Read(std::istream& stream) override;
    bool Read(wxString& string) override;
    bool Write(std::ostream& stream) const override;
    bool Write(wxString& string) const override;

    Vec3f getValue() const;
    void setValue(const Vec3f& newValue);
private:
    Vec3f data;
};
