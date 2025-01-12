#include "widgets/bone_selector_widget.h"

#include <imgui.h>
#include <iterator>

void BoneSelectorWidget::draw() {
    if (ImGui::BeginCombo(id.c_str(), react.getValue().c_str())) {
        for (const auto& choice : choices) {
            if (ImGui::Selectable(choice.c_str(), choice == react.getValue())) {
                react = choice;
            }
        }
        ImGui::EndCombo();
    }
}

void BoneSelectorWidget::setChoices(const std::vector<std::string>& val) {
    choices = val;
    if (std::find(std::begin(choices), std::end(choices), react.getValue()) == std::end(choices)) {
        react = "";
    }
}

void BoneSelectorWidget::setCurrent(const std::string& val) {
    if (std::find(std::begin(choices), std::end(choices), val) == std::end(choices)) {
        react = "";
        return;
    }

    react = val;
}

void BoneSelectorWidget::setCurrent(size_t index) {
    react = index < choices.size() ? choices[index] : "";
}
