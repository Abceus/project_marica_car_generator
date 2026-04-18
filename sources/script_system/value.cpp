#include "script_system/value.h"
#include "script_system/class.h"
#include "script_system/type_info.h"

using namespace scripting;

void Value::setValue(int newValue) {
    value = newValue;
    typeInfo = TypeInfo::getInt();
}

void Value::setValue(float newValue) {
    value = newValue;
    typeInfo = TypeInfo::getFloat();
}

void Value::setValue(const std::string& newValue) {
    value = newValue;
    typeInfo = TypeInfo::getString();
}

void Value::setValue(NullValue) {
    value = std::optional<FieldValues>{};
    typeInfo = TypeInfo{};
}

Value Value::makeObject(const Class& cls) {
    FieldValues fieldValues;
    fieldValues.reserve(cls.getFields().size());
    for (const auto& field : cls.getFields()) {
        fieldValues.push_back(field.second.constructDefaultValue());
    }

    Value result;
    result.value = fieldValues;
    result.typeInfo = &cls;
    return result;
}
