#pragma once
#include <concepts>
#include <functional>

template<class T>
concept CanCall = std::convertible_to<T, std::function<void()>>;

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