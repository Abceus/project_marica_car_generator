#include "widgets/openglview.h"
#include "render_system/element_buffer.h"
#include "render_system/mesh.h"
#include "render_system/scene_node.h"
#include "render_system/triangle_array.h"
#include "utils/math/utils.h"
#include "wx/dcclient.h"
#include "wx/event.h"
#include "wx/image.h"
#include "wx/log.h"
#include "wx/utils.h"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <memory>
#include <optional>
#include <sstream>

wxDEFINE_EVENT(OPENGL_INITED, wxCommandEvent);

OpenglView::OpenglView(wxWindow* parent) : wxGLCanvas(parent) {
    m_glRC = new wxGLContext(this);

    Bind(wxEVT_SIZE, &OpenglView::OnSize, this);

    Bind(wxEVT_PAINT, &OpenglView::OnPaint, this);

    Bind(wxEVT_CHAR_HOOK, &OpenglView::onKeyDown, this);
    Bind(wxEVT_KEY_UP, &OpenglView::onKeyUp, this);

    Bind(wxEVT_TIMER, &OpenglView::onTimer, this);

    SetWindowStyleFlag(GetWindowStyleFlag() | wxWANTS_CHARS);

    scene = std::make_shared<Scene>();
    m_renderer.setContext(m_glRC);
    m_renderer.setSurface(this);
}

void OpenglView::OnSize(wxSizeEvent& WXUNUSED(event)) {
    // Reset the OpenGL view aspect.
    // This is OK only because there is only one canvas that uses the context.
    // See the cube sample for that case that multiple canvases are made current
    // with one context.
    ResetProjectionMode();
}

void OpenglView::OnPaint(wxPaintEvent& WXUNUSED(event)) {
    // must always be here
    wxPaintDC dc(this);

    SetCurrent(*m_glRC);

    // Initialize OpenGL
    if (!inited) {
        glewInit();
        InitGL();
        ResetProjectionMode();
        inited = true;
    }

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_renderer.draw(scene);

    // Flush
    glFlush();

    // Swap
    SwapBuffers();
}

void OpenglView::ResetProjectionMode() {
    if (!IsShownOnScreen())
        return;

    // This is normally only necessary if there is more than one wxGLCanvas
    // or more than one wxGLContext in the application.
    SetCurrent(*m_glRC);

    const wxSize ClientSize = GetClientSize() * GetContentScaleFactor();

    // It's up to the application code to update the OpenGL viewport settings.
    // In order to avoid extensive context switching, consider doing this in
    // OnPaint() rather than here, though.
    glViewport(0, 0, ClientSize.x, ClientSize.y);

    scene->resizeScreen(ClientSize.x, ClientSize.y);
}

void OpenglView::InitGL() {
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto shaderProgram = m_renderer.getShaderProgram(
        ".\\resources\\shaders\\defaultvertexshader.vert",
        ".\\resources\\shaders\\defaultfragmentshader.frag");

    scene->init(shaderProgram);

    redrawTimer.SetOwner(this);
    redrawTimer.Start(1000.0f / 60.0f);

    updateTimer.SetOwner(this);
    updateTimer.Start(1000.0f / 60.0f);

    lastUpdate = std::chrono::steady_clock::now();

    wxPostEvent(this, wxCommandEvent(OPENGL_INITED));
}

void OpenglView::onKeyDown(wxKeyEvent& event) {
    wxLogDebug("Key down");
    event.Skip();
}

void OpenglView::onKeyUp(wxKeyEvent& event) {
    wxLogDebug("Key up");
}

void OpenglView::onTimer(wxTimerEvent& event) {
    if (event.GetTimer().GetId() == updateTimer.GetId()) {
        auto currentTime = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
            currentTime - lastUpdate);
        for (auto& updatable : updatables) {
            updatable->update(elapsed);
        }
        lastUpdate = currentTime;
    } else if (event.GetTimer().GetId() == redrawTimer.GetId()) {
        Refresh();
    }
}

std::weak_ptr<Scene> OpenglView::getScene() const {
    return scene;
}

Renderer& OpenglView::getRenderer() {
    return m_renderer;
}

void OpenglView::addUpdatable(const std::shared_ptr<IUpdatable>& updatable) {
    updatables.emplace(updatable);
}

void OpenglView::removeUpdatable(const std::shared_ptr<IUpdatable>& updatable) {
    updatables.erase(updatable);
}

void OpenglView::clearUpdatables() {
    updatables.clear();
}
