#pragma once

#include <map>
#include <memory>
#include <string>

#include "script_system/class.h"
#include "script_system/type_info.h"
#include "value_construtors/value_const_contructor.h"
#include "value_construtors/value_contructor.h"
#include "value_construtors/value_default_contructor.h"

namespace scripting {

class ClassManager {
public:
    ClassManager() = default;

    static ClassManager& Instance();
    ClassManager(const ClassManager&) = delete;
    ClassManager& operator=(const ClassManager&) = delete;

    const Class* getClass(const std::string& className) const;

private:
    std::map<std::string, Class> classes;
};

} // namespace scripting

inline const scripting::Class* operator""_class(const char* str, size_t) {
    return scripting::ClassManager::Instance().getClass(str);
}