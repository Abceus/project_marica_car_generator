#include "widgets/app.h"
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
#include "utils/math/matrix.h"
#include "widgets/mainwindow.h"
#include <memory>
#include <string>
#include <wx/image.h>
#include <wx/imagbmp.h>
#include <wx/imagtga.h>
#include <wx/imagpcx.h>

#include "resources/dds_info.h"


bool Application::OnInit() {
    wxImage::AddHandler(new wxTGAHandler());
    wxImage::AddHandler(new wxBMPHandler());
    wxImage::AddHandler(new wxPCXHandler());

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
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "SCENE_AMBIENT_STATIC",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

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
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROW1",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROW2",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROW3",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_POS",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROTAXIS",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_ROTANGLE", std::make_shared<FloatASEReaderNodeRule>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_SCALE",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "TM_SCALEAXIS",
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

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
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_FACE_LIST",
        std::make_shared<NumArrayASEReaderNodeRule<SetASEReaderNodeRule>>());

    ASEReaderNodeAnalyzer::Instance().addRule(
        "MESH_FACE",
        std::make_shared<ArrayASEReaderNodeRule<PairASEReaderNodeRule<
            StringASEReaderNodeRule, IntASEReaderNodeRule>, 6>>());

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
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

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
        std::make_shared<ArrayASEReaderNodeRule<FloatASEReaderNodeRule, 3>>());

    auto mainWindow = new MainWindow();
    mainWindow->Show(true);

    return true;
}
