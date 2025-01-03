#include "widgets/openglview.h"
#include "imgui.h"
#include "render_system/element_buffer.h"
#include "render_system/mesh.h"
#include "render_system/scene_node.h"
#include "render_system/triangle_array.h"
#include "utils/math/utils.h"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <memory>
#include <optional>
#include <sstream>

// wxDEFINE_EVENT(OPENGL_INITED, wxCommandEvent);

OpenglView::OpenglView() {
    // m_glRC = new wxGLContext(this);

    // Bind(wxEVT_SIZE, &OpenglView::OnSize, this);

    // Bind(wxEVT_PAINT, &OpenglView::OnPaint, this);

    // Bind(wxEVT_CHAR_HOOK, &OpenglView::onKeyDown, this);
    // Bind(wxEVT_KEY_UP, &OpenglView::onKeyUp, this);

    // Bind(wxEVT_TIMER, &OpenglView::onTimer, this);

    // SetWindowStyleFlag(GetWindowStyleFlag() | wxWANTS_CHARS);

    scene = std::make_shared<Scene>();
    // m_renderer.setContext(m_glRC);
    // m_renderer.setSurface(this);
}

void OpenglView::draw() {
    if (!inited) {
        glewInit();
        InitGL();
        inited = true;
    }

    const auto newAvailSize = ImGui::GetContentRegionAvail();
    if (prevAvailSize.x != newAvailSize.x ||
        prevAvailSize.y != newAvailSize.y) {
        OnResize(newAvailSize);
        prevAvailSize = newAvailSize;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    m_renderer.draw(scene);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Image((ImTextureID)texture, ImGui::GetContentRegionAvail(),
                 ImVec2(0, 1), ImVec2(1, 0));
}

// void OpenglView::OnSize(wxSizeEvent& WXUNUSED(event)) {
//     // Reset the OpenGL view aspect.
//     // This is OK only because there is only one canvas that uses the
//     context.
//     // See the cube sample for that case that multiple canvases are made
//     current
//     // with one context.
//     ResetProjectionMode();
// }

// void OpenglView::OnPaint(wxPaintEvent& WXUNUSED(event)) {
// // must always be here
// wxPaintDC dc(this);

// SetCurrent(*m_glRC);

// // Initialize OpenGL
// if (!inited) {
//     glewInit();
//     InitGL();
//     ResetProjectionMode();
//     inited = true;
// }

// // Clear
// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

// m_renderer.draw(scene);

// // Flush
// glFlush();

// // Swap
// SwapBuffers();
// }

void OpenglView::ResetProjectionMode() {
    // if (!IsShownOnScreen())
    //     return;

    // // This is normally only necessary if there is more than one wxGLCanvas
    // // or more than one wxGLContext in the application.
    // SetCurrent(*m_glRC);

    // const wxSize ClientSize = GetClientSize() * GetContentScaleFactor();

    // // It's up to the application code to update the OpenGL viewport
    // settings.
    // // In order to avoid extensive context switching, consider doing this in
    // // OnPaint() rather than here, though.
    // ImVec2 ClientSize = ImGui::GetWindowSize();
    // ImVec2 ClientSize = ImVec2{100, 100};
    // ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    // ImVec2 vMax = ImGui::GetWindowContentRegionMax();
    // // ImVec2 ClientSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
    // ImVec2 ClientSize = ImGui::GetContentRegionAvail();
    // glViewport(0, 0, ClientSize.x, ClientSize.y);

    // scene->resizeScreen(ClientSize.x, ClientSize.y);
}

void OpenglView::OnResize(const ImVec2& newSize) {
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, newSize.x, newSize.y, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);

    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, newSize.x,
                          newSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);

    glViewport(0, 0, newSize.x, newSize.y);

    scene->resizeScreen(newSize.x, newSize.y);
}

void OpenglView::InitGL() {

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    const int width = 100;
    const int height = 100;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,
                           texture, 0);

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT,
                              GL_RENDERBUFFER, rbo);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!"
                  << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    // glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    // glEnable(GL_DEPTH_TEST);

    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    auto shaderProgram = m_renderer.getShaderProgram(
        ".\\resources\\shaders\\defaultvertexshader.vert",
        ".\\resources\\shaders\\defaultfragmentshader.frag");

    scene->init(shaderProgram);

    // redrawTimer.SetOwner(this);
    // redrawTimer.Start(1000.0f / 60.0f);

    // updateTimer.SetOwner(this);
    // updateTimer.Start(1000.0f / 60.0f);

    // lastUpdate = std::chrono::steady_clock::now();

    if (openglInitedCallback) {
        openglInitedCallback();
    }
}

// void OpenglView::onKeyDown(wxKeyEvent& event) {
//     wxLogDebug("Key down");
//     event.Skip();
// }

// void OpenglView::onKeyUp(wxKeyEvent& event) {
//     wxLogDebug("Key up");
// }

// void OpenglView::onTimer(wxTimerEvent& event) {
// if (event.GetTimer().GetId() == updateTimer.GetId()) {
//     auto currentTime = std::chrono::steady_clock::now();
//     auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(
//         currentTime - lastUpdate);
//     for (auto& updatable : updatables) {
//         updatable->update(elapsed);
//     }
//     lastUpdate = currentTime;
// } else if (event.GetTimer().GetId() == redrawTimer.GetId()) {
//     Refresh();
// }
// }

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
