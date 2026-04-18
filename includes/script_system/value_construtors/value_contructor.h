#pragma once

#include "script_system/value.h"

namespace scripting {
class valueContructorInterface {
public:
    virtual Value construct() const = 0;
};
} // namespace scripting