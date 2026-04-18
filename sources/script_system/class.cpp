#include "script_system/class.h"

using namespace scripting;

const Field& Class::registryField(const std::string& fieldName, const Field& field) {
    auto result = fields.emplace(fieldName, field);
    return result.first->second;
}
