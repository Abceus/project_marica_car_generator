#pragma once

#include "script_system/type_info.h"
#include "script_system/type_kind.h"
#include "type_kind.h"
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace scripting {

class Value;

using FieldValues = std::vector<Value>;

class NullValue {};

class Value {
public:
    Value() = default;

    Value(int initValue) {
        setValue(initValue);
    }

    Value(float initValue) {
        setValue(initValue);
    }

    Value(const std::string& initValue) {
        setValue(initValue);
    }

    Value(NullValue null) {
        setValue(null);
    }

    void setValue(int newValue);
    void setValue(float newValue);
    void setValue(const std::string& newValue);
    void setValue(NullValue);

    int getIntValue() const {
        return std::get<int>(value);
    }

    float getFloatValue() const {
        return std::get<float>(value);
    }

    const std::string& getStringValue() const {
        return std::get<std::string>(value);
    }

    const FieldValues& getFieldValues() const {
        return std::get<std::optional<FieldValues>>(value).value();
    }

    bool isObject() const {
        return std::holds_alternative<std::optional<FieldValues>>(value);
    }

    bool isNull() const {
        return !isObject() || !std::get<std::optional<FieldValues>>(value).has_value();
    }

    bool isValid() const {
        return typeInfo.getKind() != TypeKind::INVALID;
    }

    const TypeInfo& getTypeInfo() const {
        return typeInfo;
    }

    static Value makeObject(const Class& cls);

private:
    std::variant<int, float, std::string, std::optional<FieldValues>> value;

    TypeInfo typeInfo;
};

inline Value getDefaultValueForTypeKind(TypeKind kind) {
    switch (kind) {
    case TypeKind::FLOAT:
        return Value(0.0f);
    case TypeKind::INT:
        return Value(0);
    case TypeKind::STRING:
        return Value("");
    case TypeKind::OBJECT:
        return Value(NullValue{});
    case TypeKind::INVALID:
    default:
        return Value();
    }
}

} // namespace scripting