#pragma once
#include "wx/filepicker.h"
#include <string>
#include <wx/propgrid/props.h>

class TextureArrayPGProperty : public wxPGProperty {
public:
    TextureArrayPGProperty(const wxString& label = wxPG_LABEL,
                           const wxString& name = wxPG_LABEL,
                           const wxArrayString& value = {});

    void OnSetValue() override;
    wxString GetValueAsString(int argFlags = 0) const override;
    wxVariant ChildChanged(wxVariant& thisValue, int childIndex,
                           wxVariant& childValue) const override;
    //     void resize(size_t newSize);
    //     size_t getSize() const;

    //     std::string getPath(size_t index) const;
    //     void setPath(size_t index, const std::string& newPath);

    // private:
    //     std::vector<wxFilePickerCtrl*> pickers;
    //     std::vector<std::string> pathes;
    //     wxBoxSizer* sizer = nullptr;
};
