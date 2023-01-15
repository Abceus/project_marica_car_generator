// #include <mul_opengl.h>
// #include "widgets/app.h"

// wxIMPLEMENT_APP(Application);

#include <memory>

#include <mul_context.h>
#include <mul_common.h>
#include "widgets/mainwindow.h"

int main() {
    auto& context = MulContext::getCurrentContext();
    context.init();

    auto mainWindow = MulMakeWidget<MainWindow>("Test window", Vec2I{500, 500});
    mainWindow->show();

    context.run();

    context.deinit();

    return 0;
}