#pragma once
#include "render_system/batch.h"
#include "render_system/mesh.h"
#include "render_system/scene.h"
#include "render_system/shader_program.h"
#include "utils/gl.h"
#include <memory>
#include <optional>
#include <wx/glcanvas.h>
#include <wx/timer.h>
#include "render_system/renderer.h"


class OpenglView : public wxGLCanvas {
public:
    OpenglView(wxWindow* parent);

    void OnSize(wxSizeEvent& event);
    void OnPaint(wxPaintEvent& event);

    void onKeyDown(wxKeyEvent& event);
    void onKeyUp(wxKeyEvent& event);

    void onTimer(wxTimerEvent& event);

    void onMouseEvent(wxMouseEvent& event);
    void onMouseFocusEvent(wxMouseEvent& event);

    std::weak_ptr<Scene> getScene() const;
    Renderer& getRenderer();
private:
    Renderer m_renderer;

    void InitGL();
    void ResetProjectionMode();

    bool inited = false;

    wxGLContext* m_glRC = nullptr;

    // std::shared_ptr<ShaderProgram> shaderProgram;
    // std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Scene> scene;

    wxTimer redrawTimer;

    // glm::mat4 projectionMatrix;

    // std::shared_ptr<SceneNode> newNode;

    std::optional<glm::vec2> prevMousePosition;

};