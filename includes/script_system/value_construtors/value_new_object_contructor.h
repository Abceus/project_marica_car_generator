#pragma once

#include "script_system/class_manager.h"
#include "script_system/value.h"
#include "script_system/value_construtors/value_contructor.h"

namespace scripting {
class valueNewObjectContructor : public valueContructorInterface {
public:
    // TODO: replace with Class?
    valueNewObjectContructor(const std::string& className) : className(className) {}

    Value construct() const override {
        return Value::makeObject(*ClassManager::Instance().getClass(className));
    }

private:
    std::string className;
};
} // namespace scripting