#pragma once

#include "widgets/drawable.h"
#include "widgets/react/primitive.h"
#include <string>


class StringInput : public IDrawable {
public:
    using ReactType = ReactPrimitive<std::string>;

public:
    StringInput(const std::string& label, const std::string& val)
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