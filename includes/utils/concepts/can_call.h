#pragma once
#include <concepts>
#include <functional>

template<class T, typename... Args>
concept CanCall = std::convertible_to<T, std::function<void(Args...)>>;