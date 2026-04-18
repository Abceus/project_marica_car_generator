#pragma once

#include "script_system/type_kind.h"
#include "script_system/value.h"
#include "script_system/value_construtors/value_contructor.h"

namespace scripting {
class valueDefaultContructor : public valueContructorInterface {
public:
    valueDefaultContructor(TypeKind typeKind) : typeKind(typeKind) {}

    Value construct() const override {
        return getDefaultValueForTypeKind(typeKind);
    }

private:
    TypeKind typeKind = TypeKind::INVALID;
};
} // namespace scripting