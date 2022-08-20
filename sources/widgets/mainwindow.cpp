#include "widgets/mainwindow.h"
#include "render_system/scene_node.h"
#include "widgets/configurationWidget.h"
#include "widgets/openglview.h"
#include "wx/button.h"
#include "wx/msw/window.h"
#include "wx/panel.h"
#include "wx/sizer.h"
#include "wx/splitter.h"
#include "wx/textctrl.h"

#include "resources/model.h"
#include "render_system/wireframe.h"
#include <memory>

MainWindow::MainWindow() : wxFrame(nullptr, wxID_ANY, "Main Window") {
    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    wxSplitterWindow* splitter = new wxSplitterWindow(this, wxID_ANY);
    splitter->SetSashGravity(0.5);
    mainSizer->Add(splitter, 1, wxEXPAND, 0);

    auto leftWindow = new wxWindow(splitter, wxID_ANY);

    auto leftSizer = new wxBoxSizer(wxVERTICAL);
    openglView = new OpenglView(leftWindow);
    leftSizer->Add(openglView, 1, wxEXPAND, 0);
    leftWindow->SetSizer(leftSizer);

    auto rightWindow = new wxWindow(splitter, wxID_ANY);

    auto rightSizer = new wxBoxSizer(wxVERTICAL);
    auto configurationWidget = new ConfigurationWidget(rightWindow);
    rightSizer->Add(configurationWidget, 1, wxEXPAND, 0);
    rightWindow->SetSizer(rightSizer);

    splitter->SplitVertically(leftWindow, rightWindow);

    SetSizer(mainSizer);

    mainMeshNode = std::make_shared<SceneNode>();

    auto scene = openglView->getScene();
    if (auto lockedScene = scene.lock()) {
        lockedScene->addNode(mainMeshNode);
    }

    configurationWidget->Bind(
        MESH_CHANGED, [this](const wxCommandEvent& event) {
            mainMeshNode->clear();
            auto shaderProgram = openglView->getRenderer().getShaderProgram(
                ".\\resources\\shaders\\meshvertexshader.vert",
                ".\\resources\\shaders\\meshfragmentshader.frag");
            // mainMeshNode->setShaderProgram(shaderProgram);
            auto model = Model::readPSK(event.GetString());
            // auto mesh = openglView->getRenderer().makeDrawable<Mesh>();
            auto mesh = openglView->getRenderer().makeDrawable<WireframeMesh>();
            mesh->init(model);
            mainMeshNode->addDrawable(mesh);
        });
}
