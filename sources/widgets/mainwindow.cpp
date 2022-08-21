#include "widgets/mainwindow.h"
#include "render_system/scene_node.h"
#include "utils/math/utils.h"
#include "widgets/configurationWidget.h"
#include "widgets/openglview.h"
#include "wx/button.h"
#include "wx/msw/window.h"
#include "wx/panel.h"
#include "wx/sizer.h"
#include "wx/splitter.h"
#include "wx/textctrl.h"

#include "render_system/camera/free_camera_controller.h"
#include "render_system/wireframe.h"
#include "resources/model.h"
#include <memory>

#include "physics/physicworld.h"
#include "physics/physobject.h"

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

    mainEditorCameraController =
        std::make_unique<FreeCameraController>(openglView);
    openglView->Bind(wxEVT_ENTER_WINDOW,
                     &MainWindow::onOpenglEditorMouseFocusEvent, this);
    openglView->Bind(wxEVT_LEAVE_WINDOW,
                     &MainWindow::onOpenglEditorMouseFocusEvent, this);

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
            mainModel = Model::readPSK(event.GetString());
            // auto mesh = openglView->getRenderer().makeDrawable<Mesh>();
            auto mesh = openglView->getRenderer().makeDrawable<WireframeMesh>();
            mesh->init(mainModel);
            mainMeshNode->addDrawable(mesh);
        });

    configurationWidget->Bind(
        EMULATE_BUTTON_CLICKED,
        [this](const wxCommandEvent& event) { openEmulationWindow(); });
}

void MainWindow::onOpenglEditorMouseFocusEvent(wxMouseEvent& event) {
    if (event.Leaving()) {
        SetCursor(*wxSTANDARD_CURSOR);
    } else if (event.Entering()) {
        SetCursor(*wxCROSS_CURSOR);
    }
}

void MainWindow::openEmulationWindow() {
    if (!simulateWindow) {
        simulateWindow = new wxFrame(this, wxID_ANY, "Simulation");
        auto simulateWidget = new OpenglView(simulateWindow);

        simulationEditorCameraController =
            std::make_unique<FreeCameraController>(simulateWidget);
        simulateWidget->Bind(wxEVT_ENTER_WINDOW,
                             &MainWindow::onOpenglEditorMouseFocusEvent, this);
        simulateWidget->Bind(wxEVT_LEAVE_WINDOW,
                             &MainWindow::onOpenglEditorMouseFocusEvent, this);
        auto mesh = simulateWidget->getRenderer().makeDrawable<Mesh>();
        mesh->init(mainModel);

        auto simMainMeshNode = std::make_shared<SceneNode>();
        auto shaderProgram = openglView->getRenderer().getShaderProgram(
            ".\\resources\\shaders\\meshvertexshader.vert",
            ".\\resources\\shaders\\meshfragmentshader.frag");
        simMainMeshNode->setShaderProgram(shaderProgram);

        auto scene = simulateWidget->getScene();
        if (auto lockedScene = scene.lock()) {
            lockedScene->addNode(simMainMeshNode);
        }
        simMainMeshNode->addDrawable(mesh);
        simulateWindow->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event) {
            simulateWindow = nullptr;
            simulationEditorCameraController.reset();
            event.Skip();
        });

        auto physicWorld = std::make_shared<PhysicWorld>();
        auto m_body = std::make_shared<PhysObject>(simMainMeshNode, mainModel, 1000.f,
                                                   Vec3f(10.f, 3.f, 10.f));

        m_body->setPhysic(physicWorld->addBody(m_body->getConstructionInfo()));

        simulateWidget->addUpdatable(physicWorld);
        simulateWidget->addUpdatable(m_body);

        simulateWindow->Show();
    }
}