#pragma once

#include "script_system/class.h"
#include "script_system/value.h"
#include "widgets/drawable.h"

#include <memory>
#include <string>

class ScriptObjectWidget : public IDrawable {
public:
    ScriptObjectWidget(const std::string& id)
        : id(id) {}

    void draw() override;

    void setValue(const scripting::Value& value);

private:
    scripting::Value value;
    std::vector<std::unique_ptr<IDrawable>> widgets;
    std::string id;
};