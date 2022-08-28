#include "widgets/mainwindow.h"
#include "render_system/scene_node.h"
#include "utils/math/utils.h"
#include "utils/math/vec3.h"
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

#include "utils/shapes/box.h"
#include "utils/shapes/sphere.h"

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
            mainMeshNode->setShaderProgram(shaderProgram);
            mainModel = Model::readPSK(event.GetString());
            // auto mesh = openglView->getRenderer().makeDrawable<Mesh>();
            auto mesh = openglView->getRenderer().makeDrawable<Mesh>();
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
        std::shared_ptr<SceneNode> camera;
        auto scene = simulateWidget->getScene();
        if (auto lockedScene = scene.lock()) {
            lockedScene->addNode(simMainMeshNode);

            camera = lockedScene->getActiveCamera();
        }

        auto childMainMeshNode = std::make_shared<SceneNode>();
        childMainMeshNode->setShaderProgram(shaderProgram);
        // Box childBox({10.0f, 10.0f, 10.0f});
        Sphere childBox(10.0f);
        auto childMesh = simulateWidget->getRenderer().makeDrawable<Mesh>();
        childMesh->init(childBox.getModel());
        childMainMeshNode->addDrawable(childMesh);
        childMainMeshNode->setLocation({100.0f, 0.0f, 100.0f});
        simMainMeshNode->addChild(childMainMeshNode);

        // simMainMeshNode->addChild(camera);
        camera->setLocation({-100.0f, 0.0f, 100.0f});
        camera->setRotation({0.0f, 0.0f, 0.0f});

        simMainMeshNode->addDrawable(mesh);
        simulateWindow->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event) {
            simulateWindow = nullptr;
            simulationEditorCameraController.reset();
            m_body.reset();
            event.Skip();
        });
        simulateWindow->SetWindowStyleFlag(
            simulateWindow->GetWindowStyleFlag() | wxWANTS_CHARS);
        simulateWindow->Bind(wxEVT_CHAR_HOOK, [this](wxKeyEvent& event) {
            m_body->getPhysics()->activate(true);
            auto force = 100.0f;
            btVector3 forceVec = {0.0f, 0.0f, 0.0f};
            if (event.GetKeyCode() == WXK_UP) {
                forceVec.setZ(force);
            } else if (event.GetKeyCode() == WXK_DOWN) {
                forceVec.setZ(-force);
            } else if (event.GetKeyCode() == WXK_SPACE) {
                forceVec.setY(force);
            }
            m_body->getPhysics()->applyCentralImpulse(forceVec);
        });

        auto physicWorld = std::make_shared<PhysicWorld>();
        m_body = std::make_shared<PhysObject>(simMainMeshNode, mainModel, 10.f,
                                              Vec3f(10.f, 3.f, 10.f));

        m_body->setPhysic(physicWorld->addBody(m_body->getConstructionInfo()));

        simulateWidget->addUpdatable(physicWorld);
        simulateWidget->addUpdatable(m_body);

        Box box({500.0f, 500.0f, 10.0f});
        auto colShapeGround = box.createPhysicShape();

        auto groundModel = box.getModel();

        btTransform startTransformGround;
        startTransformGround.setIdentity();
        btScalar massGround(0.f);
        bool isDynamicGround = (massGround != 0.f);

        btVector3 localInertiaGround(0, 0, 0);
        if (isDynamicGround) {
            colShapeGround->calculateLocalInertia(massGround,
                                                  localInertiaGround);
        }

        // startTransformGround.setOrigin(
        //     btVector3(btScalar(0.0), btScalar(-150.0), btScalar(300.0)));
        startTransformGround.setOrigin(Vec3f(0.0f, 0.0f, -150.0f).toBtVec3());

        auto myMotionStateGround =
            new btDefaultMotionState(startTransformGround);

        btRigidBody::btRigidBodyConstructionInfo rbInfoGround(
            massGround, myMotionStateGround, colShapeGround,
            localInertiaGround);

        auto groundMesh = simulateWidget->getRenderer().makeDrawable<Mesh>();
        groundMesh->init(groundModel);
        auto groundMeshNode = std::make_shared<SceneNode>();
        groundMeshNode->addDrawable(groundMesh);
        groundMeshNode->setShaderProgram(shaderProgram);
        groundMeshNode->setLocation(Vec3f(0.0f, 0.0f, -150.0f));
        auto ground = std::make_shared<PhysObject>(
            groundMeshNode, groundModel, 0.f, Vec3f(10.f, 3.f, 10.f));
        ground->setPhysic(physicWorld->addBody(rbInfoGround));
        // ground->setPhysic(physicWorld->addBody(ground->getConstructionInfo()));
        if (auto lockedScene = scene.lock()) {
            lockedScene->addNode(groundMeshNode);
        }
        simulateWidget->addUpdatable(ground);

        Sphere sphere(10.0f);
        auto sphereMesh = simulateWidget->getRenderer().makeDrawable<Mesh>();
        sphereMesh->init(sphere.getModel());
        auto sphereMeshNode = std::make_shared<SceneNode>();
        sphereMeshNode->setShaderProgram(shaderProgram);
        sphereMeshNode->addDrawable(sphereMesh);
        sphereMeshNode->setLocation({0.0f, 0.0f, 300.0f});
        auto spherePhys = std::make_shared<PhysObject>(
            sphereMeshNode, sphere.getModel(), 10.f, Vec3f(10.f, 3.f, 10.f));

        btTransform startSphereTransform;
        startSphereTransform.setIdentity();
        btScalar massSphere(10.f);
        bool isDynamicSphere = (massSphere != 0.f);

        auto colShapeSphere = sphere.createPhysicShape();
        btVector3 localInertiaSphere(0, 0, 0);
        if (isDynamicSphere) {
            colShapeSphere->calculateLocalInertia(massSphere,
                                                  localInertiaSphere);
        }

        // startTransformGround.setOrigin(
        //     btVector3(btScalar(0.0), btScalar(-150.0), btScalar(300.0)));
        startSphereTransform.setOrigin(
            btVector3(btScalar(0.0), btScalar(400.0), btScalar(0.0)));

        auto myMotionStateSphere =
            new btDefaultMotionState(startSphereTransform);

        btRigidBody::btRigidBodyConstructionInfo rbInfoSphere(
            massSphere, myMotionStateSphere, colShapeSphere,
            localInertiaSphere);

        spherePhys->setPhysic(physicWorld->addBody(rbInfoSphere));
        if (auto lockedScene = scene.lock()) {
            lockedScene->addNode(sphereMeshNode);
        }
        simulateWidget->addUpdatable(spherePhys);

        simulateWindow->Show();
    }
}