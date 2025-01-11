#pragma once

#include "utils/callback_collection.h"
#include <functional>


template <typename T>
class ReactPrimitive {
public:
    using CallbackCollectionType = CallbackCollection<const T&>;

public:
    ReactPrimitive() = default;

    ReactPrimitive(const T& val)
        : value(val) {}

    CallbackCollectionType& getCallbacks() {
        return callbacks;
    }

    ReactPrimitive<T>& operator=(const T& val) {
        value = val;
        callbacks.Call(val);
        return *this;
    }

    const T& getValue() const {
        return value;
    }

    void move(T&& val) {
        value = std::move(val);
    }

private:
    CallbackCollectionType callbacks;
    T value;
};