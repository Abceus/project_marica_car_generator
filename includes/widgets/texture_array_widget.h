#pragma once
#include <wx/window.h>
#include "wx/filepicker.h"
#include <string>

wxDECLARE_EVENT(TEXTURE_CHANGED, wxCommandEvent);

class TextureArrayWidget : public wxWindow {
public:
    TextureArrayWidget(wxWindow* parent);
    
    void resize(size_t newSize);
    size_t getSize() const;
    
    std::string getPath(size_t index) const;
    void setPath(size_t index, const std::string& newPath);
private:
    std::vector<wxFilePickerCtrl*> pickers;
    std::vector<std::string> pathes;
    wxBoxSizer* sizer = nullptr;
};
