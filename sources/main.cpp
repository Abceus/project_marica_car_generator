// #include <mul_opengl.h>
// #include "widgets/app.h"

// wxIMPLEMENT_APP(Application);

#include <memory>

#include <mul_context.h>
#include "widgets/mainwindow.h"

int main() {
    auto& context = MulContext::getCurrentContext();
    context.init();

    auto mainWindow = new MainWindow();
    mainWindow->init();
    mainWindow->show();

    context.run();

    context.deinit();

    return 0;
}