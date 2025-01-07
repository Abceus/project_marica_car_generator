#pragma once

#include <functional>

template <typename T>
class ReactPrimitive {
public:
    using ValueChangedCallbackType = std::function<void(const T&)>;

public:
    ReactPrimitive(const T& val)
        : value(val) {}

    void setCallback(const ValueChangedCallbackType& val) {
        callback = val;
    }

    ReactPrimitive<T>& operator=(const T& val) {
        value = val;
        if (callback) {
            callback(value);
        }
        return *this;
    }

    const T& getValue() const {
        return value;
    }

private:
    ValueChangedCallbackType callback;
    T value;
};