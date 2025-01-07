#pragma once

#include "widgets/opengl_glfw_window.h"
#include <memory>

class Application {
public:
    void run();

private:
    bool init();
    void mainLoop();
    void deinit();

    std::unique_ptr<OpenglGlfwWindow> mainWindow;
};
