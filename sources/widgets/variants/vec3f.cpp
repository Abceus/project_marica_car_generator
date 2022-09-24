#include "widgets/variants/vec3f.h"
#include <sstream>
#include <string>

Vec3fVariantData::Vec3fVariantData(const Vec3f& newData) : data(newData) {}

Vec3fVariantData::Vec3fVariantData(const Vec3fVariantData& other) {
    data = other.data;
}

wxVariantData* Vec3fVariantData::Clone() const {
    return new Vec3fVariantData(data);
}

bool Vec3fVariantData::Eq(wxVariantData& data) const {
    if (data.GetType() == GetType()) {
        wxString otherData;
        wxString myData;
        data.Write(otherData);
        Write(myData);
        return otherData == myData;
    }
    return false;
}

wxString Vec3fVariantData::GetType() const {
    return "Vec3f";
}

bool Vec3fVariantData::Read(std::istream& stream) {
    stream >> data;
    return true;
}

bool Vec3fVariantData::Read(wxString& string) {
    std::stringstream ss;
    ss << string;
    ss >> data;
    return true;
}

bool Vec3fVariantData::Write(std::ostream& stream) const {
    stream << data;
    return true;
}

bool Vec3fVariantData::Write(wxString& string) const {
    std::stringstream ss;
    ss << data;
    string = ss.str();
    return true;
}

Vec3f Vec3fVariantData::getValue() const {
    return data;
}

void Vec3fVariantData::setValue(const Vec3f& newValue) {
    data = newValue;
}