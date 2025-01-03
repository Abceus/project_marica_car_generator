#include "widgets/mainwindow.h"
#include "render_system/scene_node.h"
#include "resources/dds_info.h"
#include "resources/wireframe_model.h"
#include "utils/math/angle.h"
#include "utils/math/quaternion.h"
#include "utils/math/rot3.h"
#include "utils/math/utils.h"
#include "utils/math/vec3.h"
#include "utils/shapes/convex_hull.h"
#include "utils/shapes/shape.h"
#include "widgets/configuration_widget.h"
#include "widgets/event_data/indexed_texture.h"
#include "widgets/opengl_glfw_window.h"
#include "widgets/openglview.h"

#include "render_system/camera/free_camera_controller.h"
#include "render_system/wireframe.h"
#include "resources/model.h"
#include <memory>
#include <vector>

#ifdef WITH_PHYSICS
#include "physics/physicworld.h"
#include "physics/physobject.h"
#endif

#include "utils/shapes/box.h"
#include "utils/shapes/sphere.h"

#include "widgets/event_data/float.h"

#include <imgui.h>

MainWindow::MainWindow() : OpenglGlfwWindow("Main Window") {
    openglView = std::make_unique<OpenglView>();
    configurationWidget = std::make_unique<ConfigurationWidget>();
    //     leftSizer->Add(openglView, 1, wxEXPAND, 0);
    //     leftWindow->SetSizer(leftSizer);

    //     auto rightWindow = new wxWindow(splitter, wxID_ANY);

    //     auto rightSizer = new wxBoxSizer(wxVERTICAL);
    //     auto configurationWidget = new ConfigurationWidget(rightWindow);
    //     rightSizer->Add(configurationWidget, 1, wxEXPAND, 0);
    //     rightWindow->SetSizer(rightSizer);

    //     splitter->SplitVertically(leftWindow, rightWindow);

    //     SetSizer(mainSizer);

    //     mainEditorCameraController =
    //         std::make_unique<FreeCameraController>(openglView);
    //     openglView->Bind(wxEVT_ENTER_WINDOW,
    //                      &MainWindow::onOpenglEditorMouseFocusEvent, this);
    //     openglView->Bind(wxEVT_LEAVE_WINDOW,
    //                      &MainWindow::onOpenglEditorMouseFocusEvent, this);

    //     openglView->Bind(OPENGL_INITED, [this](wxCommandEvent& event) {
    //         auto shaderProgram = openglView->getRenderer().getShaderProgram(
    //             ".\\resources\\shaders\\meshvertexshader.vert",
    //             ".\\resources\\shaders\\meshfragmentshader.frag");

    //         mainNode = std::make_shared<SceneNode>();
    mainMesh = openglView->getRenderer().makeDrawable<Mesh>();
    //         mainNode->setShaderProgram(shaderProgram);
    //         mainNode->addDrawable(mainMesh);
    //         mainCollisionMesh =
    //             openglView->getRenderer().makeDrawable<WireframeMesh>();
    //         mainNode->addDrawable(mainCollisionMesh);

    //         auto testNode = std::make_shared<SceneNode>();
    //         testNode->setOverlay(true);
    //         mainNode->addChild(testNode);
    //         WireframeModel wireframeModel;
    //         wireframeModel.vertices = {{-10.0, 0.0, 0.0, 0.0, 0.0, 0}, {10.0,
    //         0.0, 0.0, 0.0, 0.0, 0}, {0.0, -10.0, 0.0, 0.0, 0.0, 0},
    //         {0.0, 10.0, 0.0, 0.0, 0.0, 0}, {0.0, 0.0, -10.0, 0.0, 0.0, 0},
    //         {0.0, 0.0, 10.0, 0.0, 0.0, 0}};
    //         wireframeModel.edges = {{0, 1}, {2, 3}, {4, 5}};
    //         auto testMesh =
    //         openglView->getRenderer().makeDrawable<WireframeMesh>();
    //         testMesh->init(wireframeModel, {1.0f, 0.0f, 0.0f});
    //         testNode->addDrawable(testMesh);

    //         tireCollisionMesh =
    //             openglView->getRenderer().makeDrawable<WireframeMesh>();
    //         for (auto& wheel : wheelSteerMeshNodes) {
    //             wheel = std::make_shared<SceneNode>();
    //             mainNode->addChild(wheel);
    //             wheel->addDrawable(tireCollisionMesh);
    //         }

    //         for (auto& wheel : wheelEngMeshNodes) {
    //             wheel = std::make_shared<SceneNode>();
    //             mainNode->addChild(wheel);
    //             wheel->addDrawable(tireCollisionMesh);
    //         }

    //         auto scene = openglView->getScene();
    //         if (auto lockedScene = scene.lock()) {
    //             lockedScene->addNode(mainNode);
    //         }
    //     });

    configurationWidget->setMeshChangedCallback(
        [this](const std::string& filePath) {
            mainModel = Model::readPSK(filePath);
            mainMesh->init(mainModel);
            configurationWidget->resizeTextureArray(mainModel.materials.size());
            for (size_t i = 0; i < mainModel.materials.size(); ++i) {
                configurationWidget->setTexture(i, mainModel.materials[i]);
            }
        });

    //     configurationWidget->Bind(SKIN_CHANGED, [this](wxCommandEvent& event)
    //     {
    //         if (auto IndexedTextureDataData =
    //                 static_cast<IndexedTextureData*>(event.GetClientObject()))
    //                 {
    //             if (wxFileExists(IndexedTextureDataData->path.string())) {
    //                 auto newTexture =
    //                 openglView->getRenderer().makeDrawable<Texture>(); auto
    //                 extension =
    //                 IndexedTextureDataData->path.extension().string();
    //                 std::transform(extension.begin(), extension.end(),
    //                 extension.begin(),
    //                     [](unsigned char c){ return std::tolower(c); });
    //                 auto currentContext =
    //                 openglView->getRenderer().pushContextScoped();
    //                 if(extension == ".dds") {
    //                     newTexture->init(DDSInfo::loadDDS(IndexedTextureDataData->path.string()));
    //                 }
    //                 else {
    //                     newTexture->init(wxImage(IndexedTextureDataData->path.string()));
    //                 }
    //                 mainMesh->setTexture(newTexture,
    //                 IndexedTextureDataData->index);
    //             }
    //         }
    //     });

    //     configurationWidget->Bind(
    //         COLLISION_CHANGED, [this](const wxCommandEvent& event) {
    //             // auto models = Model::readASE(event.GetString());
    //             // if (!models.empty()) {
    //             //     mainCollision = WireframeModel::fromModel(models[0]);
    //             // } else {
    //             //     mainCollision = WireframeModel();
    //             // }
    //             auto model = Model::readPSK(event.GetString());
    //             mainCollision = WireframeModel::fromModel(model);
    //             mainCollisionMesh->init(mainCollision);
    //         });

    //     configurationWidget->Bind(
    //         TIRE_CHANGED, [this](const wxCommandEvent& event) {
    //             // auto models = Model::readASE(event.GetString());
    //             // if (!models.empty()) {
    //             //     tireCollision = WireframeModel::fromModel(models[0]);
    //             // } else {
    //             //     tireCollision = WireframeModel();
    //             // }
    //             auto model = Model::readPSK(event.GetString());
    //             tireCollision = WireframeModel::fromModel(model);
    //             tireCollisionMesh->init(tireCollision);
    //         });

    //     configurationWidget->Bind(
    //         WHEEL_STEER_ACROSS_CHANGED, [this](wxCommandEvent& event) {
    //             if (auto floatData =
    //                     static_cast<FloatData*>(event.GetClientObject())) {
    //                 // setWheelSteerAcross(floatData->value);
    //                 // mainMesh->rotateBone("LeftFrontTIRE",
    //                 //
    //                 Quaternion::fromEulerAngles(Rotor3(Angle::fromDegrees(floatData->value))));
    //                 mainMesh->transposeBone("LeftFrontTIRE",
    //                 {floatData->value});
    //             }
    //         });

    //     configurationWidget->Bind(
    //         WHEEL_STEER_ALONG_CHANGED, [this](wxCommandEvent& event) {
    //             if (auto floatData =
    //                     static_cast<FloatData*>(event.GetClientObject())) {
    //                 // setWheelSteerAlong(floatData->value);
    //                 mainMesh->rotateBone("LeftFrontTIRE",
    //                                      Quaternion::fromEulerAngles(Rotor3(
    //                                          Angle::fromDegrees(0.0),
    //                                          Angle::fromDegrees(floatData->value),
    //                                          Angle::fromDegrees(0.0))));
    //             }
    //         });

    //     configurationWidget->Bind(
    //         WHEEL_ENG_ACROSS_CHANGED, [this](wxCommandEvent& event) {
    //             if (auto floatData =
    //                     static_cast<FloatData*>(event.GetClientObject())) {
    //                 setWheelEngAcross(floatData->value);
    //             }
    //         });

    //     configurationWidget->Bind(
    //         WHEEL_ENG_ALONG_CHANGED, [this](wxCommandEvent& event) {
    //             if (auto floatData =
    //                     static_cast<FloatData*>(event.GetClientObject())) {
    //                 setWheelEngAlong(floatData->value);
    //             }
    //         });

    //     configurationWidget->Bind(WHEEL_VERT_CHANGED, [this](
    //                                                       wxCommandEvent&
    //                                                       event) {
    //         if (auto floatData =
    //         static_cast<FloatData*>(event.GetClientObject())) {
    //             setWheelVert(floatData->value);
    //         }
    //     });

    // #ifdef WITH_PHYSICS
    //     configurationWidget->Bind(
    //         EMULATE_BUTTON_CLICKED,
    //         [this](const wxCommandEvent& event) { openEmulationWindow(); });
    // #endif
}

// void MainWindow::onOpenglEditorMouseFocusEvent(wxMouseEvent& event) {
//     if (event.Leaving()) {
//         SetCursor(*wxSTANDARD_CURSOR);
//     } else if (event.Entering()) {
//         SetCursor(*wxCROSS_CURSOR);
//     }
// }

void MainWindow::onDraw() {
#ifdef IMGUI_HAS_VIEWPORT
    ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->GetWorkPos());
    ImGui::SetNextWindowSize(viewport->GetWorkSize());
    ImGui::SetNextWindowViewport(viewport->ID);
#else
    ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
#endif
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    bool open = true;
    ImGui::Begin("Main Frame", &open,
                 ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize);

    ImGui::BeginChild("Left Main Subframe", ImVec2(currentSplitterPosition, 0),
                      true);
    openglView->draw();
    ImGui::EndChild();

    ImGui::SameLine();
    ImGui::InvisibleButton("Main Vertical Splitter",
                           ImVec2(8.0f, ImGui::GetContentRegionAvail().y));
    if (ImGui::IsItemActive())
        currentSplitterPosition += ImGui::GetIO().MouseDelta.x;
    ImGui::SameLine();

    ImGui::BeginChild("Right Main Subframe", ImVec2(0, 0), true);
    configurationWidget->draw();
    ImGui::EndChild();

    ImGui::End();
    ImGui::PopStyleVar();
}

void MainWindow::setMainModel(const Model& model) {
    mainModel = model;
}

void MainWindow::setMainCollision(const Model& model) {
    mainCollision = WireframeModel::fromModel(model);
}

void MainWindow::setTireCollision(const Model& model) {
    tireCollision = WireframeModel::fromModel(model);
}

void MainWindow::setWheelSteerAlong(float value) {
    for (auto& wheel : wheelSteerMeshNodes) {
        auto leftSteerWheelLocation = wheel->getLocation();
        leftSteerWheelLocation.setX(value);
        wheel->setLocation(leftSteerWheelLocation);
    }
}

void MainWindow::setWheelSteerAcross(float value) {
    auto leftSteerWheelLocation = wheelSteerMeshNodes[0]->getLocation();
    leftSteerWheelLocation.setY(value);
    wheelSteerMeshNodes[0]->setLocation(leftSteerWheelLocation);

    auto rightSteerWheelLocation = wheelSteerMeshNodes[1]->getLocation();
    rightSteerWheelLocation.setY(-value);
    wheelSteerMeshNodes[1]->setLocation(rightSteerWheelLocation);
}

void MainWindow::setWheelEngAlong(float value) {
    for (auto& wheel : wheelEngMeshNodes) {
        auto leftSteerWheelLocation = wheel->getLocation();
        leftSteerWheelLocation.setX(value);
        wheel->setLocation(leftSteerWheelLocation);
    }
}

void MainWindow::setWheelEngAcross(float value) {
    auto leftEngWheelLocation = wheelEngMeshNodes[0]->getLocation();
    leftEngWheelLocation.setY(value);
    wheelEngMeshNodes[0]->setLocation(leftEngWheelLocation);

    auto rightEngWheelLocation = wheelEngMeshNodes[1]->getLocation();
    rightEngWheelLocation.setY(-value);
    wheelEngMeshNodes[1]->setLocation(rightEngWheelLocation);
}

void MainWindow::setWheelVert(float value) {
    for (auto& wheel : wheelSteerMeshNodes) {
        auto leftSteerWheelLocation = wheel->getLocation();
        leftSteerWheelLocation.setZ(value);
        wheel->setLocation(leftSteerWheelLocation);
    }

    for (auto& wheel : wheelEngMeshNodes) {
        auto leftSteerWheelLocation = wheel->getLocation();
        leftSteerWheelLocation.setZ(value);
        wheel->setLocation(leftSteerWheelLocation);
    }
}

#ifdef WITH_PHYSICS
void MainWindow::openEmulationWindow() {
    if (!simulateWindow) {
        simulateWindow = new wxFrame(this, wxID_ANY, "Simulation");
        auto simulateWidget = new OpenglView(simulateWindow);
        auto scene = simulateWidget->getScene();
        auto physicWorld = std::make_shared<PhysicWorld>();
        simulateWidget->addUpdatable(physicWorld);

        simulationEditorCameraController =
            std::make_unique<FreeCameraController>(simulateWidget);
        simulateWidget->Bind(wxEVT_ENTER_WINDOW,
                             &MainWindow::onOpenglEditorMouseFocusEvent, this);
        simulateWidget->Bind(wxEVT_LEAVE_WINDOW,
                             &MainWindow::onOpenglEditorMouseFocusEvent, this);

        auto simMainNode = std::make_shared<SceneNode>();
        if (auto lockedScene = scene.lock()) {
            lockedScene->addNode(simMainNode);
        }
        auto simMainCollisionMesh =
            simulateWidget->getRenderer().makeDrawable<WireframeMesh>();
        simMainCollisionMesh->init(mainCollision);
        simMainNode->addDrawable(simMainCollisionMesh);

        auto mainCollisionShape =
            std::make_shared<ConvexHull>(mainCollision.vertices);
        mainPhysic =
            std::make_shared<PhysObject>(simMainNode, mainCollisionShape);
        mainPhysic->setMass(500.0f);
        mainPhysic->setPhysic(
            physicWorld->addBody(mainPhysic->getConstructionInfo()));
        simulateWidget->addUpdatable(mainPhysic);

        auto shaderProgram = simulateWidget->getRenderer().getShaderProgram(
            ".\\resources\\shaders\\meshvertexshader.vert",
            ".\\resources\\shaders\\meshfragmentshader.frag");
        auto simMainMeshNode = std::make_shared<SceneNode>();
        simMainMeshNode->setShaderProgram(shaderProgram);
        auto simMainMesh = simulateWidget->getRenderer().makeDrawable<Mesh>();
        simMainMesh->init(mainModel);
        simMainMeshNode->addDrawable(simMainMesh);
        simMainNode->addChild(simMainMeshNode);

        auto wheelMesh =
            simulateWidget->getRenderer().makeDrawable<WireframeMesh>();
        wheelMesh->init(tireCollision);

        auto tireShape = std::make_shared<ConvexHull>(tireCollision.vertices);

        std::vector<std::array<std::shared_ptr<SceneNode>, 2>> wheels = {
            wheelSteerMeshNodes, wheelEngMeshNodes};

        for (const auto& nodes : wheels) {
            for (const auto& wheel : nodes) {
                auto wheelNode = std::make_shared<SceneNode>();
                wheelNode->setLocation(wheel->getLocation());
                wheelNode->addDrawable(wheelMesh);
                if (auto lockedScene = scene.lock()) {
                    lockedScene->addNode(wheelNode);
                }

                auto tirePhysBody =
                    std::make_shared<PhysObject>(wheelNode, tireShape);
                tirePhysBody->setMass(3.0f);
                tirePhysBody->setFriction(1.0f);
                tirePhysBody->setPhysic(
                    physicWorld->addBody(tirePhysBody->getConstructionInfo()));
                tirePhysBody->getPhysics()->setIgnoreCollisionCheck(
                    mainPhysic->getPhysics().get(), true);
                simulateWidget->addUpdatable(tirePhysBody);

                auto bodyJointTransform = btTransform::getIdentity();
                bodyJointTransform.setOrigin(
                    wheelNode->getLocation().toBtVec3());

                auto constraint =
                    physicWorld->addConstraint<btSliderConstraint>(
                        *mainPhysic->getPhysics(), *tirePhysBody->getPhysics(),
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

        std::shared_ptr<SceneNode> camera;
        if (auto lockedScene = scene.lock()) {
            camera = lockedScene->getActiveCamera();
        }

        camera->setLocation({-100.0f, 0.0f, 100.0f});
        camera->setRotation({0.0f, 0.0f, 0.0f});

        simulateWindow->Bind(wxEVT_CLOSE_WINDOW, [this](wxCloseEvent& event) {
            simulateWindow = nullptr;
            simulationEditorCameraController.reset();
            mainPhysic.reset();
            event.Skip();
        });
        simulateWindow->SetWindowStyleFlag(
            simulateWindow->GetWindowStyleFlag() | wxWANTS_CHARS);
        simulateWindow->Bind(wxEVT_CHAR_HOOK, [this](wxKeyEvent& event) {
            mainPhysic->getPhysics()->activate(true);
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
            mainPhysic->getPhysics()->applyCentralImpulse(forceVec.toBtVec3());
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

        simulateWindow->Show();
    }
}
#endif