#include "script_system/loader.h"
#include "script_system/class.h"
#include "script_system/value.h"

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

ClassBuilder& Loader::registryClass(const std::string& className) {
    return classes.emplace_back(className);
}
