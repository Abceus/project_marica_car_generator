#pragma once

#include "widgets/react/primitive.h"
#include <string>
#include <vcruntime.h>
#include <vector>

class BoneSelectorWidget {
private:
    using ReactType = ReactPrimitive<std::string>;

public:
    BoneSelectorWidget(const std::string& id)
        : id(id) {}

    void draw();

    void setChoices(const std::vector<std::string>& choices);
    void setCurrent(const std::string& val);
    void setCurrent(size_t index);

    ReactType& getReact() {
        return react;
    }

    const ReactType& getReact() const {
        return react;
    }

private:
    ReactType react;
    std::vector<std::string> choices;
    std::string id;
};