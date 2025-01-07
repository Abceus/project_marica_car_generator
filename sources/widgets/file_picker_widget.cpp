#include "widgets/file_picker_widget.h"
#include "imgui.h"
#include <ImGuiFileDialog.h>
#include <filesystem>

FilePickerWidget::FilePickerWidget(const std::string& uniqueId)
    : id(uniqueId) {
    config.path = ".";
}

void FilePickerWidget::draw() {
    if (ImGui::Button(!pathReact.getValue().string().empty() ? pathReact.getValue().string().c_str() : defaultTitle.c_str())) {
        ImGuiFileDialog::Instance()->OpenDialog(id.c_str(),
                                                dialogTitle.c_str(),
                                                fileExtensions.c_str(),
                                                config);
    }

    if (ImGuiFileDialog::Instance()->Display(id.c_str())) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            pathReact = std::filesystem::path{ImGuiFileDialog::Instance()->GetFilePathName()};

            if (saveLastPath) {
                config.path = ImGuiFileDialog::Instance()->GetCurrentPath();
            }
        }

        ImGuiFileDialog::Instance()->Close();
    }
}