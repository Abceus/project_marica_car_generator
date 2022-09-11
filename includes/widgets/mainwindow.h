#pragma once
#include "physics/physobject.h"
#include "render_system/camera/camera_controller.h"
#include "render_system/scene_node.h"
#include "render_system/wireframe.h"
#include "widgets/openglview.h"
#include "wx/msw/window.h"
#include <memory>
#include <wx/frame.h>

class MainWindow : public wxFrame {
public:
    MainWindow();

    void onOpenglEditorMouseFocusEvent(wxMouseEvent& event);

    void setMainModel(const Model& model);
    void setMainCollision(const Model& model);
    void setTireCollision(const Model& model);

    void setWheelSteerAlong(float value);
    void setWheelSteerAcross(float value);

    void setWheelEngAlong(float value);
    void setWheelEngAcross(float value);

    void setWheelVert(float value);

private:
    void openEmulationWindow();

    Model mainModel;
    WireframeModel mainCollision;
    WireframeModel tireCollision;

    OpenglView* openglView = nullptr;
    wxWindow* simulateWindow = nullptr;

    std::shared_ptr<SceneNode> mainNode;
    std::shared_ptr<Mesh> mainMesh;
    std::shared_ptr<WireframeMesh> mainCollisionMesh;
    std::shared_ptr<WireframeMesh> tireCollisionMesh;

    std::array<std::shared_ptr<SceneNode>, 2> wheelSteerMeshNodes;
    std::array<std::shared_ptr<SceneNode>, 2> wheelEngMeshNodes;
    //

    std::unique_ptr<CameraController> mainEditorCameraController;
    std::unique_ptr<CameraController> simulationEditorCameraController;

    std::shared_ptr<PhysObject> mainPhysic;

    std::vector<Vec3f> tirePositions = {{100.0f, 100.0f, -50.0f},
                                        {-100.0f, 100.0f, -50.0f},
                                        {100.0f, -100.0f, -50.0f},
                                        {-100.0f, -100.0f, -50.0f}};

    std::vector<std::weak_ptr<btSliderConstraint>> contsts;
};
