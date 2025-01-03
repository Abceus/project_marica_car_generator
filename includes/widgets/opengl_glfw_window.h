#pragma once

#include "utils/gl.h"

#include "GLFW/glfw3.h"
#include "widgets/drawable.h"
#include <string>

class OpenglGlfwWindow : public IDrawable {
public:
    OpenglGlfwWindow(const std::string& title);
    virtual ~OpenglGlfwWindow();

    bool init();
    void deinit();

    void draw() final;
    bool isOpened() const;

protected:
    virtual void onDraw() = 0;

private:
    GLFWwindow* window = nullptr;
    std::string title;
};