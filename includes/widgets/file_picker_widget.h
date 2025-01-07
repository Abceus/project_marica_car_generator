#pragma once

#include <ImGuiFileDialog.h>
#include <filesystem>
#include <functional>
#include <string>

class FilePickerWidget {
public:
    using FilePickedCallbackType = std::function<void(const std::filesystem::path&)>;

public:
    FilePickerWidget(const std::string& uniqueId);

    void draw();

    void setFilePickedCallback(const FilePickedCallbackType& callback) {
        filePickedCallback = callback;
    }

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

    void setPickedPath(const std::filesystem::path& path) {
        pickedPath = path;
    }

    std::filesystem::path getPickedPath() const {
        return pickedPath;
    }

private:
    std::string id;
    std::string defaultTitle;
    std::filesystem::path pickedPath;
    std::string dialogTitle = "Select File";
    std::string fileExtensions = "";
    FilePickedCallbackType filePickedCallback;
    IGFD::FileDialogConfig config;
    bool saveLastPath = true;
};
