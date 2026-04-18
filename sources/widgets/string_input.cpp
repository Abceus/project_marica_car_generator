#include "widgets/string_input.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>

void StringInput::draw() {
    auto val = react.getValue();
    if (ImGui::InputText(label.c_str(), &val)) {
        react = val;
    }
}