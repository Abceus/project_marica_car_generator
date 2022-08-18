#include "widgets/mainwindow.h"
#include "widgets/openglview.h"
#include "widgets/configurationWidget.h"
#include "wx/msw/window.h"
#include "wx/sizer.h"
#include "wx/splitter.h"
#include "wx/textctrl.h"
#include "wx/button.h"
#include "wx/panel.h"

#include "resources/model.h"
 
MainWindow::MainWindow()
    : wxFrame(nullptr, wxID_ANY, "Main Window") {
        auto mainSizer = new wxBoxSizer(wxVERTICAL);
        wxSplitterWindow *splitter = new wxSplitterWindow(this, wxID_ANY);
        splitter->SetSashGravity(0.5);
        mainSizer->Add(splitter, 1, wxEXPAND, 0);

        auto leftWindow = new wxWindow(splitter, wxID_ANY);

        auto leftSizer = new wxBoxSizer(wxVERTICAL);
        auto openglView = new OpenglView(leftWindow);
        leftSizer->Add(openglView, 1, wxEXPAND,0);
        leftWindow->SetSizer(leftSizer);

        auto rightWindow = new wxWindow(splitter, wxID_ANY);

        auto rightSizer = new wxBoxSizer(wxVERTICAL);
        auto configurationWidget = new ConfigurationWidget(rightWindow);
        rightSizer->Add(configurationWidget, 1, wxEXPAND, 0);
        rightWindow->SetSizer(rightSizer);

        splitter->SplitVertically(leftWindow, rightWindow);

        SetSizer(mainSizer);

        configurationWidget->Bind(MESH_CHANGED, [](const wxCommandEvent& event) {
            auto model = Model::readPSK(event.GetString());
        });
}
