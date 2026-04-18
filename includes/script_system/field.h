#pragma once

#include "value_construtors/value_contructor.h"
#include "value_construtors/value_default_contructor.h"
#include <memory>
#include <script_system/type_info.h>
#include <script_system/value.h>
#include <string>

namespace scripting {

class Field {
public:
    Field() = default;

    Field(const std::string& name, const TypeInfo& typeInfo) : name(name), typeInfo(typeInfo) {}

    Field(const std::string& name, const TypeInfo& typeInfo, const std::shared_ptr<valueContructorInterface>& defaultValueContructor)
        : name(name), typeInfo(typeInfo), defaultValueContructor(defaultValueContructor) {}

    void setName(const std::string& newName) {
        name = newName;
    }

    void setTypeInfo(const TypeInfo& newTypeInfo) {
        typeInfo = newTypeInfo;
    }

    void setDefaultValueContructor(std::shared_ptr<valueContructorInterface>&& newValue) {
        defaultValueContructor = std::move(newValue);
    }

    const std::string& getName() const {
        return name;
    }

    const TypeInfo& getTypeInfo() const {
        return typeInfo;
    }

    Value constructDefaultValue() const {
        return defaultValueContructor->construct();
    }

private:
    std::string name;
    TypeInfo typeInfo;
    std::shared_ptr<valueContructorInterface> defaultValueContructor = std::make_shared<valueDefaultContructor>(typeInfo.getKind());
};

} // namespace scripting