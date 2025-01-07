#include "widgets/float_input.h"

#include <imgui.h>

void FloatInput::draw() {
    auto val = react.getValue();
    if (ImGui::InputFloat(label.c_str(), &val, 0.01f, 1.0f)) {
        react = val;
    }
}