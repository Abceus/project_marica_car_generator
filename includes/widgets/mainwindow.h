#pragma once
#include <wx/frame.h>
#include "render_system/scene_node.h"
#include "widgets/openglview.h"

class MainWindow : public wxFrame
{
public:
    MainWindow();
private:
    std::shared_ptr<SceneNode> mainMeshNode;
    OpenglView* openglView = nullptr;
};
