#pragma once
#include "widgets/configuration_widget.h"
#include "widgets/opengl_glfw_window.h"
#ifdef WITH_PHYSICS
#include "physics/physobject.h"
#endif
#include "render_system/camera/camera_controller.h"
#include "render_system/scene_node.h"
#include "render_system/wireframe.h"
#include "widgets/drawable.h"
#include "widgets/openglview.h"
#include <memory>

class MainWindow : public OpenglGlfwWindow {
public:
    MainWindow();

    void onDraw() override;

    // void onOpenglEditorMouseFocusEvent(wxMouseEvent& event);

    void setMainModel(const Model& model);
    void setMainCollision(const Model& model);
    void setTireCollision(const Model& model);

    void setWheelSteerAlong(float value);
    void setWheelSteerAcross(float value);

    void setWheelEngAlong(float value);
    void setWheelEngAcross(float value);

    void setWheelVert(float value);

private:
#ifdef WITH_PHYSICS
    void openEmulationWindow();
#endif

    Model mainModel;
    WireframeModel mainCollision;
    WireframeModel tireCollision;

    std::unique_ptr<OpenglView> openglView;
    std::unique_ptr<ConfigurationWidget> configurationWidget;
    // wxWindow* simulateWindow = nullptr;

    std::shared_ptr<SceneNode> mainNode;
    std::shared_ptr<Mesh> mainMesh;
    std::shared_ptr<WireframeMesh> mainCollisionMesh;
    std::shared_ptr<WireframeMesh> tireCollisionMesh;

    std::array<std::shared_ptr<SceneNode>, 2> wheelSteerMeshNodes;
    std::array<std::shared_ptr<SceneNode>, 2> wheelEngMeshNodes;
    //

    std::unique_ptr<CameraController> mainEditorCameraController;
    std::unique_ptr<CameraController> simulationEditorCameraController;

    std::vector<Vec3f> tirePositions = {{100.0f, 100.0f, -50.0f},
                                        {-100.0f, 100.0f, -50.0f},
                                        {100.0f, -100.0f, -50.0f},
                                        {-100.0f, -100.0f, -50.0f}};

    float currentSplitterPosition = 500.0f;

#ifdef WITH_PHYSICS
    std::vector<std::weak_ptr<btSliderConstraint>> contsts;

    std::shared_ptr<PhysObject> mainPhysic;
#endif
};
