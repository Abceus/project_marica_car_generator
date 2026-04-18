#pragma once

#include "script_system/value.h"
#include "script_system/value_construtors/value_contructor.h"

namespace scripting {
class valueConstContructor : public valueContructorInterface {
public:
    valueConstContructor(const Value& value) : value(value) {}

    Value construct() const override {
        return value;
    }

private:
    Value value;
};
} // namespace scripting