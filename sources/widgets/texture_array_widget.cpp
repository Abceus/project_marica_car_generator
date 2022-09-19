#include "widgets/texture_array_widget.h"
#include "widgets/event_data/indexed_texture.h"
#include "wx/msw/window.h"

wxDEFINE_EVENT(TEXTURE_CHANGED, wxCommandEvent);

TextureArrayWidget::TextureArrayWidget(wxWindow* parent)
    : wxWindow(parent, wxID_ANY) {
    sizer = new wxBoxSizer(wxVERTICAL);
    // sizer->SetSizeHints(this);
    SetSizer(sizer);
}

void TextureArrayWidget::resize(size_t newSize) {
    if (pickers.size() > newSize) {
        for (size_t i = newSize; i < pickers.size(); ++i) {
            setPath(i, "");
            pickers[i]->Destroy();
        }
    }

    pickers.resize(newSize);
    if (newSize > pathes.size()) {
        for (size_t i = pathes.size(); i < newSize; ++i) {
            pickers[i] =
                new wxFilePickerCtrl(this, wxID_ANY, wxEmptyString,
                                     "Select texture", "DDS (*.dds)|*.dds|TGA (*.tga)|*.tga|BMP (*.bmp)|*.bmp|PCX (*.pcx)|*.pcx");

            pickers[i]->SetWindowStyleFlag(wxFLP_DEFAULT_STYLE | wxFLP_OPEN |
                                           wxFLP_FILE_MUST_EXIST);
            sizer->Add(pickers[i], 0, wxEXPAND | wxTOP | wxBOTTOM, 10);
            pickers[i]->Bind(wxEVT_FILEPICKER_CHANGED,
                             [this, i](const wxFileDirPickerEvent& event) {
                                 setPath(i, event.GetPath());
                             });
        }
    }
    if (auto parent = GetParent()) {
        if (auto parentSizer = parent->GetSizer()) {
            parentSizer->Layout();
        }
    }
    pathes.resize(newSize);
}

size_t TextureArrayWidget::getSize() const {
    return pickers.size();
}

std::string TextureArrayWidget::getPath(size_t index) const {
    return pathes[index];
}

void TextureArrayWidget::setPath(size_t index, const std::string& newPath) {
    pathes[index] = newPath;
    pickers[index]->SetPath(newPath);
    auto data = new IndexedTexture();
    data->index = index;
    data->path = newPath;
    wxCommandEvent newEvent(TEXTURE_CHANGED);
    newEvent.SetClientObject(data);
    wxPostEvent(this, newEvent);
}