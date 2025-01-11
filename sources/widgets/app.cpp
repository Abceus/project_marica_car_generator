#include "widgets/app.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "resources/ase_reader/ase_reader.h"
#include "resources/ase_reader/ase_reader_node_analyzer.h"
#include "resources/ase_reader/rules/array.h"
#include "resources/ase_reader/rules/dict.h"
#include "resources/ase_reader/rules/float.h"
#include "resources/ase_reader/rules/int.h"
#include "resources/ase_reader/rules/num_array.h"
#include "resources/ase_reader/rules/pair.h"
#include "resources/ase_reader/rules/set.h"
#include "resources/ase_reader/rules/string.h"
#include "resources/ase_reader/rules/value.h"
#include "resources/dds_info.h"
#include "sound/context.h"
#include "utils/math/matrix.h"
#include "widgets/mainwindow.h"
#include <memory>
#include <string>

#include "resources/dds_info.h"

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

static void glfw_error_callback(int error, const char* description) {
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

void Application::run() {
    if (!init()) {
        return;
    }

    mainLoop();
    deinit();
}

bool Application::init() {
    ASEReaderNodeAnalyzer::Instance().addRule(
        "3DSMAX_ASCIIEXPORT", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "COMMENT", std::make_shared<StringASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE", std::make_shared<DictASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE_FILENAME", std::make_shared<StringASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE_FIRSTFRAME", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE_LASTFRAME", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE_FRAMESPEED", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE_TICKSPERFRAME", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE_BACKGROUND_STATIC",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE_AMBIENT_STATIC",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MATERIAL_LIST", std::make_shared<DictASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MATERIAL_COUNT", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "GEOMOBJECT", std::make_shared<DictASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "NODE_NAME", std::make_shared<StringASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "NODE_TM", std::make_shared<DictASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "INHERIT_POS",
        std::make_shared<ArrayASEReaderNodeRule<IntASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "INHERIT_ROT",
        std::make_shared<ArrayASEReaderNodeRule<IntASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "INHERIT_SCL",
        std::make_shared<ArrayASEReaderNodeRule<IntASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROW0",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROW1",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROW2",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROW3",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_POS",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROTAXIS",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROTANGLE", std::make_shared<FloatASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_SCALE",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_SCALEAXIS",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_SCALEAXISANG", std::make_shared<FloatASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH", std::make_shared<DictASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TIMEVALUE", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_NUMVERTEX", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_NUMFACES", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_VERTEX_LIST",
        std::make_shared<NumArrayASEReaderNodeRule<SetASEReaderNodeRule>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_NUMTVERTEX", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_VERTEX",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_FACE_LIST",
        std::make_shared<NumArrayASEReaderNodeRule<SetASEReaderNodeRule>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_FACE", std::make_shared<ArrayASEReaderNodeRule<
                         PairASEReaderNodeRule<StringASEReaderNodeRule,
                                               IntASEReaderNodeRule>,
                         6>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_SMOOTHING", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_MTLID", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_NUMTVERTEX", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_TVERTLIST",
        std::make_shared<NumArrayASEReaderNodeRule<SetASEReaderNodeRule>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_TVERT",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_NUMTVFACES", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_TFACELIST",
        std::make_shared<NumArrayASEReaderNodeRule<SetASEReaderNodeRule>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_TFACE",
        std::make_shared<ArrayASEReaderNodeRule<IntASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_NORMALS",
        std::make_shared<NumArrayASEReaderNodeRule<SetASEReaderNodeRule>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_FACENORMAL",
        std::make_shared<PairASEReaderNodeRule<
            ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>,
            ArrayASEReaderNodeRule<ValueASEReaderNodeRule, 3>>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_VERTEXNORMAL",
        std::make_shared<PairASEReaderNodeRule<
            IntASEReaderNodeRule,
            ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "PROP_MOTIONBLUR", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "PROP_CASTSHADOW", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "PROP_RECVSHADOW", std::make_shared<IntASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "WIREFRAME_COLOR",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule,
                                                3>>());

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return false;

        // Decide GL+GLSL versions
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100 (WebGL 1.0)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(IMGUI_IMPL_OPENGL_ES3)
    // GL ES 3.0 + GLSL 300 es (WebGL 2.0)
    const char* glsl_version = "#version 300 es";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);           // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    // glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+
    // only glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // 3.0+ only
#endif

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    openalContext = std::make_unique<OpenALContext>();
    openalContext->init();

    mainWindow = std::make_unique<MainWindow>();

    mainWindow->init();

    ImGui_ImplOpenGL3_Init(glsl_version);
    glfwSwapInterval(1); // Enable vsync

    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    return true;
}

void Application::mainLoop() {
#ifdef __EMSCRIPTEN__
    // For an Emscripten build we are disabling file-system access, so let's not
    // attempt to do a fopen() of the imgui.ini file. You may manually call
    // LoadIniSettingsFromMemory() to load settings from your own storage.
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (mainWindow->isOpened())
#endif
    {
        glfwPollEvents();
        mainWindow->draw();
    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
}

void Application::deinit() {
    if (mainWindow) {
        mainWindow->deinit();
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwTerminate();

    openalContext->deinit();
}
