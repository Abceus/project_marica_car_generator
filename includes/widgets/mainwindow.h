#pragma once
#include "widgets/mul_opengl_canvas.h"
#include "widgets/openglview.h"
#include <filesystem>
#ifdef WITH_PHYSICS
#include "physics/physobject.h"
#endif
// #include "render_system/camera/camera_controller.h"
#include "render_system/scene_node.h"
#include "render_system/wireframe.h"
#include "render_system/mesh.h"
// #include "widgets/openglview.h"
// #include "wx/msw/window.h"
#include <memory>
// #include <wx/frame.h>
#include <mul_window.h>
#include <mul_callback_owner.h>
#include "widgets/configuration_widget.h"

class MainWindow : public MulWindow {
public:
    void onInit() override;

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

    OpenglView::OpenglInitedCallbackType openglInitedCallback;
    std::shared_ptr<OpenglView> openglView;
    // wxWindow* simulateWindow = nullptr;

    std::shared_ptr<SceneNode> mainNode;
    std::shared_ptr<Mesh> mainMesh;
    std::shared_ptr<WireframeMesh> mainCollisionMesh;
    std::shared_ptr<WireframeMesh> tireCollisionMesh;

    std::array<std::shared_ptr<SceneNode>, 2> wheelSteerMeshNodes;
    std::array<std::shared_ptr<SceneNode>, 2> wheelEngMeshNodes;

    // std::unique_ptr<CameraController> mainEditorCameraController;
    // std::unique_ptr<CameraController> simulationEditorCameraController;

    std::vector<Vec3f> tirePositions = {{100.0f, 100.0f, -50.0f},
                                        {-100.0f, 100.0f, -50.0f},
                                        {100.0f, -100.0f, -50.0f},
                                        {-100.0f, -100.0f, -50.0f}};

#ifdef WITH_PHYSICS
    std::vector<std::weak_ptr<btSliderConstraint>> contsts;

    std::shared_ptr<PhysObject> mainPhysic;
#endif

    std::shared_ptr<MulCallbackOwner<>> click;
    std::shared_ptr<MulCallbackOwner<bool>> checkboxClick;
    std::shared_ptr<MulCallbackOwner<size_t, std::string>> comboboxClick;
    std::shared_ptr<MulCallbackOwner<float>> floatInputClick;
    std::shared_ptr<MulCallbackOwner<std::filesystem::path>> filePickerClick;

    ConfigurationWidget::MeshChangedCallbackType meshChangeHandler;
};
