#pragma once

#include "widgets/react/primitive.h"
#include <string>

class FloatInput {
public:
    using ReactType = ReactPrimitive<float>;

public:
    FloatInput(const std::string& label, float val)
        : label(label), react(val) {}

    void draw();

    void setLabel(const std::string& value) {
        label = value;
    }

    ReactType& getReact() {
        return react;
    }

    const ReactType& getReact() const {
        return react;
    }

    void setCallback(const ReactType::ValueChangedCallbackType& callback) {
        react.setCallback(callback);
    }

private:
    std::string label;
    ReactType react;
};