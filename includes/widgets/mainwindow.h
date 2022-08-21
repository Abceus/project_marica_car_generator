#pragma once
#include <memory>
#include <wx/frame.h>
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
    OpenglView* openglView = nullptr;

    wxWindow* simulateWindow = nullptr;

    Model mainModel;

    std::unique_ptr<CameraController> mainEditorCameraController;
    std::unique_ptr<CameraController> simulationEditorCameraController;
};
