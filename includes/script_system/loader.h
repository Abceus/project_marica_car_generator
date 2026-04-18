#pragma once

#include <memory>
#include <string>

#include "script_system/field.h"
#include "script_system/type_info.h"
#include "script_system/value_construtors/value_const_contructor.h"
#include "script_system/value_construtors/value_contructor.h"

namespace scripting {
namespace builders {

class FieldBuilder {
public:
    FieldBuilder(const std::string& fieldName, const TypeInfo& typeInfo) : fieldName(fieldName), typeInfo(typeInfo) {}

    FieldBuilder& setDefaultValueContructor(const std::shared_ptr<valueContructorInterface>& newValue) {
        defaultValueContructor = newValue;
        return *this;
    }

    FieldBuilder& setDefaultValue(const Value& defaultValue) {
        defaultValueContructor = std::make_shared<valueConstContructor>(defaultValue);
        return *this;
    }

    Field build() const;

    // private:
    std::string fieldName;
    TypeInfo typeInfo;
    std::shared_ptr<valueContructorInterface> defaultValueContructor = std::make_shared<valueDefaultContructor>(typeInfo.getKind());
};

using field = FieldBuilder;

class ClassBuilder {
public:
    ClassBuilder(const std::string& className) : className(className) {}

    ClassBuilder() = delete;

    ClassBuilder(const ClassBuilder& other) = delete;
    ClassBuilder(ClassBuilder&& other) = default;

    ClassBuilder& operator=(const ClassBuilder& other) = delete;
    ClassBuilder& operator=(ClassBuilder&& other) = default;

    ClassBuilder& registryFields(std::initializer_list<FieldBuilder> fields);
    ClassBuilder& registryField(const std::string& fieldName, const TypeInfo& typeInfo);
    ClassBuilder& setDefaultProperties(const Value& newDefaultProperties);

private:
    std::string className;
    std::vector<FieldBuilder> fields;
};

} // namespace builders

class Loader {
public:
    builders::ClassBuilder& registryClass(const std::string& className);

private:
    std::vector<builders::ClassBuilder> classes;
};
} // namespace scripting