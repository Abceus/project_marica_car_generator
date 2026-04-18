#include "widgets/script_object_widget.h"
#include "script_system/class.h"
#include "script_system/type_info.h"
#include "script_system/type_kind.h"
#include "script_system/value.h"
#include "widgets/drawable.h"
#include "widgets/float_input.h"
#include "widgets/int_input.h"
#include "widgets/string_input.h"

#include <imgui.h>
#include <memory>

void ScriptObjectWidget::draw() {

    ImGui::PushID(id.c_str());
    if (ImGui::TreeNode(id.c_str())) {
        if (!value.isObject()) {
            ImGui::LabelText("", "Non object value");
        } else if (value.isNull()) {
            ImGui::LabelText("", "Null value");
        } else {
            for (const auto& widget : widgets) {
                widget->draw();
            }
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void ScriptObjectWidget::setValue(const scripting::Value& newValue) {
    value = newValue.isObject() ? newValue : scripting::Value();

    widgets.clear();
    if (!value.isValid() || value.isNull()) {
        return;
    }

    const auto& cls = value.getTypeInfo().getClass();
    widgets.reserve(cls->getFields().size());

    for (const auto& field : cls->getFields()) {
        std::unique_ptr<IDrawable> newWidget = [&field]() -> std::unique_ptr<IDrawable> {
            switch (field.second.getTypeInfo().getKind()) {
            case scripting::TypeKind::INT:
                return std::make_unique<IntInput>(field.first, field.second.constructDefaultValue().getIntValue());
            case scripting::TypeKind::FLOAT:
                return std::make_unique<FloatInput>(field.first, field.second.constructDefaultValue().getFloatValue());
            case scripting::TypeKind::STRING:
                return std::make_unique<StringInput>(field.first, field.second.constructDefaultValue().getStringValue());
            case scripting::TypeKind::OBJECT: {
                auto result = std::make_unique<ScriptObjectWidget>(field.first);
                result->setValue(field.second.constructDefaultValue());
                return result;
            }
            default:
                return nullptr;
            }
        }();

        widgets.emplace_back(std::move(newWidget));
    }
}
