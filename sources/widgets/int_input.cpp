#include "widgets/int_input.h"

#include <imgui.h>

void IntInput::draw() {
    auto val = react.getValue();
    if (ImGui::InputInt(label.c_str(), &val, 1, 100)) {
        react = val;
    }
}