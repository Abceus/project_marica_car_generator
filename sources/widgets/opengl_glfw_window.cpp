#include "widgets/opengl_glfw_window.h"
#include "GLFW/glfw3.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <corecrt_wstdio.h>
#include <stdio.h>

OpenglGlfwWindow::OpenglGlfwWindow(const std::string& title)
    : title(title) {}

OpenglGlfwWindow::~OpenglGlfwWindow() {
    deinit();
}

bool OpenglGlfwWindow::init() {
    // Create window with graphics context
    window = glfwCreateWindow(1280, 720, title.c_str(), nullptr, nullptr);
    if (window == nullptr)
        return false;

    glfwMakeContextCurrent(window);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCallbacks(window, "#canvas");
#endif

    return true;
}

void OpenglGlfwWindow::deinit() {
    if (window == nullptr)
        return;

    glfwDestroyWindow(window);
}

void OpenglGlfwWindow::draw() {
    if (window == nullptr) {
        return;
    }

    if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0) {
        ImGui_ImplGlfw_Sleep(10);
        return;
    }

    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    onDraw();

    ImGui::Render();
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);

    static const ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
                 clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    glfwSwapBuffers(window);
}

bool OpenglGlfwWindow::isOpened() const {
    if (window == nullptr)
        return false;

    return !glfwWindowShouldClose(window);
}