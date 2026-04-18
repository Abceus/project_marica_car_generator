#include "script_system/class_manager.h"
#include "script_system/value.h"
#include <memory>

using namespace scripting;
using namespace builders;

Field FieldBuilder::build() const {
    return Field(fieldName, typeInfo, defaultValueContructor);
}

ClassBuilder& ClassBuilder::registryFields(std::initializer_list<FieldBuilder> fields) {
    for (const auto& field : fields) {
        cls.registryField(field.fieldName, field.build());
    }
    return *this;
}

ClassBuilder& ClassBuilder::registryField(const std::string& fieldName, const TypeInfo& typeInfo) {
    cls.registryField(fieldName, Field(fieldName, typeInfo));
    return *this;
}

ClassBuilder& ClassBuilder::setDefaultProperties(const Value& newDefaultProperties) {
    cls.setDefaultProperties(newDefaultProperties);
    return *this;
}

Class& ClassBuilder::getClass() {
    return cls;
}

ClassManager& ClassManager::Instance() {
    static ClassManager instance;
    return instance;
}

ClassBuilder ClassManager::registryClass(const std::string& className) {
    auto result = classes.emplace(className, className);
    return ClassBuilder(result.first->second);
}

const Class* ClassManager::getClass(const std::string& className) const {
    auto result = classes.find(className);
    if (result == std::end(classes)) {
        return nullptr;
    }
    return &result->second;
}