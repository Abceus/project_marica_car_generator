#include "widgets/openglview.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "render_system/element_buffer.h"
#include "render_system/mesh.h"
#include "render_system/scene_node.h"
#include "render_system/shader_program.h"
#include "render_system/triangle_array.h"
#include "utils/math/utils.h"
#include <chrono>
#include <cmath>
#include <cstddef>
#include <fstream>
#include <memory>
#include <optional>
#include <sstream>

void OpenglView::draw() {
    auto currentTime = std::chrono::steady_clock::now();
    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastUpdate);
    if (elapsed.count() >= 1000.0f / 60.0f) {
        for (auto& updatable : updatables) {
            updatable->update(elapsed);
        }
        lastUpdate = currentTime;
    }

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
    ResetProjectionMode();

    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    scene->draw();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    ImGui::Image((ImTextureID)texture, ImGui::GetContentRegionAvail(),
                 ImVec2(0, 1), ImVec2(1, 0));

    if (ImGui::IsItemHovered() && cameraController && scene->getActiveCamera()) {
        cameraController->update(scene->getActiveCamera());
    }

    for (const auto& key : keysForEventsEmit) {
        if (ImGui::IsKeyPressed(key, false)) {
            if (pressKeyCallback) {
                pressKeyCallback(key);
            }
        }

        if (ImGui::IsKeyReleased(key)) {
            if (releaseKeyCallback) {
                releaseKeyCallback(key);
            }
        }
    }
}

void OpenglView::ResetProjectionMode() {
    const auto availRegion = ImGui::GetContentRegionAvail();

    glViewport(0, 0, availRegion.x, availRegion.y);
    scene->resizeScreen(availRegion.x, availRegion.y);
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
    glBindTexture(GL_TEXTURE_2D, 0);
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

    auto shaderProgram = std::make_shared<ShaderProgram>();
    shaderProgram->init(
        std::filesystem::path(".\\resources\\shaders\\defaultvertexshader.vert"),
        std::filesystem::path(".\\resources\\shaders\\defaultfragmentshader.frag"));

    scene->init(shaderProgram);

    lastUpdate = std::chrono::steady_clock::now();

    if (openglInitedCallback) {
        openglInitedCallback();
    }
}

std::weak_ptr<Scene> OpenglView::getScene() const {
    return scene;
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
