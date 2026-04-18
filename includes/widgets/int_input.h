#pragma once

#include "widgets/react/primitive.h"
#include "widgets/drawable.h"
#include <string>

class IntInput : public IDrawable {
public:
    using ReactType = ReactPrimitive<int>;

public:
    IntInput(const std::string& label, int val)
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