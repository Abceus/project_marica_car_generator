#include "widgets/mainwindow.h"
#include "render_system/scene_node.h"
#include "resources/wireframe_model.h"
#include "utils/math/angle.h"
#include "utils/math/quaternion.h"
#include "utils/math/rot3.h"
#include "utils/math/utils.h"
#include "utils/math/vec3.h"
#include "utils/shapes/convex_hull.h"
#include "utils/shapes/shape.h"
#include "widgets/configurationWidget.h"
#include "widgets/openglview.h"
#include "wx/button.h"
#include "wx/event.h"
#include "wx/msw/window.h"
#include "wx/panel.h"
#include "wx/sizer.h"
#include "wx/splitter.h"
#include "wx/textctrl.h"

#include "render_system/camera/free_camera_controller.h"
#include "render_system/wireframe.h"
#include "resources/model.h"
#include <memory>
#include <vector>

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
    size_t i = 0;
    for (auto& tireMeshNode : tireMeshNodes) {
        tireMeshNode = std::make_shared<SceneNode>();
        tireMeshNode->setLocation(tirePositions[i]);
        ++i;
    }

    auto scene = openglView->getScene();
    if (auto lockedScene = scene.lock()) {
        lockedScene->addNode(mainMeshNode);
        for (auto& tireMeshNode : tireMeshNodes) {
            lockedScene->addNode(tireMeshNode);
        }
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
        COLLISION_CHANGED, [this](const wxCommandEvent& event) {
            mainMeshNode->clear();
            auto shaderProgram = openglView->getRenderer().getShaderProgram(
                ".\\resources\\shaders\\meshvertexshader.vert",
                ".\\resources\\shaders\\meshfragmentshader.frag");
            // mainMeshNode->setShaderProgram(shaderProgram);
            auto models = Model::readASE(event.GetString());
            for (const auto& model : models) {
                // auto mesh = openglView->getRenderer().makeDrawable<Mesh>();
                auto mesh =
                    openglView->getRenderer().makeDrawable<WireframeMesh>();
                mesh->init(WireframeModel::fromModel(model));
                mainMeshNode->addDrawable(mesh);
            }
        });

    configurationWidget->Bind(
        TIRE_CHANGED, [this](const wxCommandEvent& event) {
            auto shaderProgram = openglView->getRenderer().getShaderProgram(
                ".\\resources\\shaders\\meshvertexshader.vert",
                ".\\resources\\shaders\\meshfragmentshader.frag");
            tireModel = Model::readPSK(event.GetString());
            auto mesh = openglView->getRenderer().makeDrawable<Mesh>();
            mesh->init(tireModel);
            for (auto& tireMeshNode : tireMeshNodes) {
                tireMeshNode->clear();
                tireMeshNode->setShaderProgram(shaderProgram);
                tireMeshNode->addDrawable(mesh);
            }
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
        auto mesh = simulateWidget->getRenderer().makeDrawable<WireframeMesh>();
        // mesh->init(mainModel);

        auto simMainMeshNode = std::make_shared<SceneNode>();
        auto shaderProgram = openglView->getRenderer().getShaderProgram(
            ".\\resources\\shaders\\meshvertexshader.vert",
            ".\\resources\\shaders\\meshfragmentshader.frag");
        // simMainMeshNode->setShaderProgram(shaderProgram);
        std::shared_ptr<SceneNode> camera;
        auto scene = simulateWidget->getScene();
        if (auto lockedScene = scene.lock()) {
            lockedScene->addNode(simMainMeshNode);

            camera = lockedScene->getActiveCamera();
        }

        // auto childMainMeshNode = std::make_shared<SceneNode>();
        // childMainMeshNode->setShaderProgram(shaderProgram);
        // // Box childBox({10.0f, 10.0f, 10.0f});
        // Sphere childBox(10.0f);
        // auto childMesh = simulateWidget->getRenderer().makeDrawable<Mesh>();
        // childMesh->init(childBox.getModel());
        // childMainMeshNode->addDrawable(childMesh);
        // childMainMeshNode->setLocation({100.0f, 0.0f, 100.0f});
        // simMainMeshNode->addChild(childMainMeshNode);

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
            Vec3f forceVec;
            if (event.GetKeyCode() == WXK_UP) {
                // forceVec.setX(force);
                for (auto& c : contsts) {
                    if (auto constraint = c.lock()) {
                        constraint->setTargetAngMotorVelocity(150.0f);
                    }
                }
            } else if (event.GetKeyCode() == WXK_DOWN) {
                // forceVec.setX(-force);
                for (auto& c : contsts) {
                    if (auto constraint = c.lock()) {
                        constraint->setTargetAngMotorVelocity(-150.0f);
                    }
                }
            } else if (event.GetKeyCode() == WXK_LEFT) {
                forceVec.setY(force);
            } else if (event.GetKeyCode() == WXK_RIGHT) {
                forceVec.setY(-force);
            } else if (event.GetKeyCode() == WXK_SPACE) {
                // forceVec.setZ(force);

                for (auto& c : contsts) {
                    if (auto constraint = c.lock()) {
                        constraint->setPoweredAngMotor(false);
                    }
                }
            } else if (event.GetKeyCode() == WXK_CONTROL) {
                forceVec.setZ(-force);
            }
            m_body->getPhysics()->applyCentralImpulse(forceVec.toBtVec3());
        });
        simulateWidget->Bind(wxEVT_KEY_UP, [this](wxKeyEvent& event) {
            if (event.GetKeyCode() == WXK_UP) {
                // forceVec.setX(force);
                for (auto& c : contsts) {
                    if (auto constraint = c.lock()) {
                        constraint->setTargetAngMotorVelocity(0.0f);
                    }
                }
            } else if (event.GetKeyCode() == WXK_DOWN) {
                // forceVec.setX(-force);
                for (auto& c : contsts) {
                    if (auto constraint = c.lock()) {
                        constraint->setTargetAngMotorVelocity(0.0f);
                    }
                }
            } else if (event.GetKeyCode() == WXK_SPACE) {
                // forceVec.setZ(force);

                for (auto& c : contsts) {
                    if (auto constraint = c.lock()) {
                        constraint->setPoweredAngMotor(true);
                    }
                }
            }
        });

        auto physicWorld = std::make_shared<PhysicWorld>();
        auto bodyShape = std::make_shared<ConvexHull>(mainModel);
        // mesh->init(bodyShape->getModel());
        mesh->init(WireframeModel::fromModel(bodyShape->getModel()));
        // simMainMeshNode->setRotation(Quaternion::fromEulerAngles(
        //     Rotor3(Angle::fromDegrees(180.0f), Angle(), Angle())));
        m_body = std::make_shared<PhysObject>(simMainMeshNode, bodyShape);
        m_body->setMass(500.0f);

        m_body->setPhysic(physicWorld->addBody(m_body->getConstructionInfo()));

        simulateWidget->addUpdatable(physicWorld);
        simulateWidget->addUpdatable(m_body);

        auto box = std::make_shared<Box>(Vec3f{1500.0f, 1500.0f, 10.0f});

        auto groundModel = box->getModel();

        auto groundMesh = simulateWidget->getRenderer().makeDrawable<Mesh>();
        groundMesh->init(groundModel);
        auto groundMeshNode = std::make_shared<SceneNode>();
        groundMeshNode->addDrawable(groundMesh);
        groundMeshNode->setShaderProgram(shaderProgram);
        groundMeshNode->setLocation(Vec3f(0.0f, 0.0f, -150.0f));
        auto ground = std::make_shared<PhysObject>(groundMeshNode, box);
        ground->setFriction(1.0f);
        ground->setPhysic(physicWorld->addBody(ground->getConstructionInfo()));
        if (auto lockedScene = scene.lock()) {
            lockedScene->addNode(groundMeshNode);
        }
        simulateWidget->addUpdatable(ground);

        // auto sphere = std::make_shared<Sphere>(10.0f);
        // auto sphereMesh = simulateWidget->getRenderer().makeDrawable<Mesh>();
        // sphereMesh->init(sphere->getModel());
        // auto sphereMeshNode = std::make_shared<SceneNode>();
        // sphereMeshNode->setShaderProgram(shaderProgram);
        // sphereMeshNode->addDrawable(sphereMesh);
        // sphereMeshNode->setLocation({0.0f, 0.0f, 400.0f});
        // // sphereMeshNode->addChild(camera);
        // auto spherePhys =
        //     std::make_shared<PhysObject>(sphereMeshNode, sphere, 10.f);

        // spherePhys->setPhysic(
        //     physicWorld->addBody(spherePhys->getConstructionInfo()));
        // if (auto lockedScene = scene.lock()) {
        //     lockedScene->addNode(sphereMeshNode);
        // }
        // simulateWidget->addUpdatable(spherePhys);

        {
            auto tireShape = std::make_shared<ConvexHull>(tireModel);
            auto tireMesh =
                simulateWidget->getRenderer().makeDrawable<WireframeMesh>();
            tireMesh->init(WireframeModel::fromModel(tireShape->getModel()));

            for (const auto& tirePosition : tirePositions) {
                auto tireMeshNode = std::make_shared<SceneNode>();
                tireMeshNode->setLocation(tirePosition);
                tireMeshNode->addDrawable(tireMesh);
                auto scene = simulateWidget->getScene();
                if (auto lockedScene = scene.lock()) {
                    lockedScene->addNode(tireMeshNode);
                }

                auto tirePhysBody =
                    std::make_shared<PhysObject>(tireMeshNode, tireShape);
                tirePhysBody->setMass(3.0f);
                tirePhysBody->setFriction(1.0f);

                tirePhysBody->setPhysic(
                    physicWorld->addBody(tirePhysBody->getConstructionInfo()));

                simulateWidget->addUpdatable(tirePhysBody);

                auto bodyJointTransform = btTransform::getIdentity();
                bodyJointTransform.setOrigin(tirePosition.toBtVec3());

                auto constraint =
                    physicWorld->addConstraint<btSliderConstraint>(
                        *m_body->getPhysics(), *tirePhysBody->getPhysics(),
                        bodyJointTransform, btTransform::getIdentity(), true);

                constraint->setUpperAngLimit(
                    Angle::fromDegrees(180.0f).getRadians());
                constraint->setLowerAngLimit(
                    Angle::fromDegrees(-180.0f).getRadians());
                constraint->setLowerLinLimit(0.0f);
                constraint->setUpperLinLimit(0.0f);
                constraint->setMaxAngMotorForce(15000.0f);
                constraint->setPoweredAngMotor(true);
                contsts.emplace_back(constraint);
            }
        }

        simulateWindow->Show();
    }
}