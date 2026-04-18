#pragma once

#include "widgets/react/primitive.h"
#include "widgets/drawable.h"
#include <string>

class FloatInput : public IDrawable {
public:
    using ReactType = ReactPrimitive<float>;

public:
    FloatInput(const std::string& label, float val)
        : label(label), react(val) {}

    void draw() override;

    void setLabel(const std::string& value) {
        label = value;
    }

    ReactType& getReact() {
        return react;
    }

    const ReactType& getReact() const {
        return react;
    }

private:
    std::string label;
    ReactType react;
};