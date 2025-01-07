#pragma once

#include "widgets/react/primitive.h"
#include <ImGuiFileDialog.h>
#include <filesystem>
#include <functional>
#include <string>

class FilePickerWidget {
public:
    using ReactType = ReactPrimitive<std::filesystem::path>;

public:
    FilePickerWidget(const std::string& uniqueId);

    void draw();

    void setDefaultTitle(const std::string& value) {
        defaultTitle = value;
    }

    void setDialogTitle(const std::string& value) {
        dialogTitle = value;
    }

    void setFileExtensions(const std::string& value) {
        fileExtensions = value;
    }

    void setDefaultPath(const std::string& value) {
        config.path = value;
    }

    void setSaveLastPath(bool value) {
        saveLastPath = value;
    }

    ReactType& getReact() {
        return pathReact;
    }

private:
    std::string id;
    std::string defaultTitle;
    ReactType pathReact{""};
    std::string dialogTitle = "Select File";
    std::string fileExtensions = "";
    IGFD::FileDialogConfig config;
    bool saveLastPath = true;
};
