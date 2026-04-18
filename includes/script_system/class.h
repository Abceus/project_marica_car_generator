#pragma once

#include <map>
#include <string>

#include "script_system/field.h"
#include "script_system/type_info.h"
#include "script_system/value.h"

namespace scripting {

class Class {
public:
    using Fields = std::map<std::string, Field>;

public:
    Class(const std::string& className) : name(className) {}

    const Field& registryField(const std::string& fieldName, const Field& field);

    void setDefaultProperties(const Value& newDefaultProperties) {
        defaultProperties = newDefaultProperties;
    }

    const Fields& getFields() const {
        return fields;
    }

private:
    std::string name;
    Fields fields;
    Value defaultProperties;
};

} // namespace scripting