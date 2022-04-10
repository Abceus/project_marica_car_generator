#pragma once
#include <functional>
#include "utils/concepts/can_call.h"

class [[nodiscard]] ScopeGuard
{
public:
    template<CanCall Function>
    ScopeGuard(const Function& deleter_);

    ScopeGuard(const ScopeGuard& copy) = delete;
    ScopeGuard& operator= ( const ScopeGuard& ) = delete;
private:
    std::function<void()> deleter;
};

template<CanCall Function>
ScopeGuard::ScopeGuard(const Function& deleter_) : deleter(deleter_)
{
}