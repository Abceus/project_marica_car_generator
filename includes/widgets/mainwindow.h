#pragma once
#include <memory>
#include <wx/frame.h>
#include "physics/physobject.h"
#include "render_system/scene_node.h"
#include "widgets/openglview.h"
#include "wx/msw/window.h"
#include "render_system/camera/camera_controller.h"

class MainWindow : public wxFrame
{
public:
    MainWindow();

    void onOpenglEditorMouseFocusEvent(wxMouseEvent& event);
private:
    void openEmulationWindow();
    std::shared_ptr<SceneNode> mainMeshNode;
    std::array<std::shared_ptr<SceneNode>, 4> tireMeshNodes;
    OpenglView* openglView = nullptr;

    wxWindow* simulateWindow = nullptr;

    Model mainModel;
    Model tireModel;

    std::unique_ptr<CameraController> mainEditorCameraController;
    std::unique_ptr<CameraController> simulationEditorCameraController;

    std::shared_ptr<PhysObject> m_body;

    std::vector<Vec3f> tirePositions = {
        {100.0f, 100.0f, -50.0f},
        {-100.0f, 100.0f, -50.0f},
        {100.0f, -100.0f, -50.0f},
        {-100.0f, -100.0f, -50.0f}
    };

     std::vector<std::weak_ptr<btSliderConstraint>> contsts;
};
