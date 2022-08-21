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
        InitGL();
        ResetProjectionMode();
        inited = true;
    }

    // Clear
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Transformations
    // glLoadIdentity();
    // glTranslatef(0.0f, 0.0f, -20.0f);
    // GLfloat m[4][4];
    // build_rotmatrix( m, m_gldata.quat );
    // glMultMatrixf( &m[0][0] );

    // m_renderer.Render();
    // batch->draw();
    // shaderProgram->bind();
    // glm::mat4 model = glm::mat4(1.0f);
    // model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
    // shaderProgram->setUniform("model", model);

    // glm::vec3 direction;
    // direction.x = cos(glm::radians(cameraRotation.y)) *
    // cos(glm::radians(cameraRotation.x)); direction.y =
    // sin(glm::radians(cameraRotation.x)); direction.z =
    // sin(glm::radians(cameraRotation.y)) *
    // cos(glm::radians(cameraRotation.x)); glm::vec3 cameraDirection =
    // glm::normalize(direction); glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    // glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    // glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    // auto view = glm::lookAt(cameraPosition,
    //         cameraPosition+cameraDirection,
    //         cameraUp);

    // glm::mat4 view = glm::mat4(1.0f);
    // view = glm::translate(view, glm::vec3(0.0f, 0.0f, 0.0f));
    // shaderProgram->setUniform("view", glm::mat4(1.0f));

    // shaderProgram->setUniform("projection", projectionMatrix);

    // mesh->draw();
    // for (auto i = newNode->drawableBegin(); i != newNode->drawableEnd(); i++)
    // {
    // (*i)->draw();
    // }
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

    // projectionMatrix = glm::perspective(
    //     glm::radians(90.0f), static_cast<float>(ClientSize.x) / ClientSize.y,
    //     1.0f, 10000.0f);

    scene->resizeScreen(ClientSize.x, ClientSize.y);

    // glMatrixMode(GL_PROJECTION);
    // glLoadIdentity();
    // gluPerspective(360, double(ClientSize.x) / ClientSize.y, 1, 10000);
    // glMatrixMode(GL_MODELVIEW);
    // glLoadIdentity();
}

void OpenglView::InitGL() {
    glewInit();

    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // m_renderer.setContext(m_glRC);
    // m_renderer.setSurface(this);

    // std::ifstream fragmentStream(
    //     "D:\\Documents\\gits\\project_marica_car_"
    //     "generator\\resources\\shaders\\meshfragmentshader.frag");
    // std::stringstream fragmentBuffer;
    // fragmentBuffer << fragmentStream.rdbuf();

    // std::ifstream vertexStream(
    //     "D:\\Documents\\gits\\project_marica_car_"
    //     "generator\\resources\\shaders\\meshvertexshader.vert");
    // std::stringstream vertexShaderBuffer;
    // vertexShaderBuffer << vertexStream.rdbuf();

    // auto shaderProgram = std::make_shared<ShaderProgram>();
    // shaderProgram->init(vertexShaderBuffer.str(), fragmentBuffer.str());

    // auto vertexShader =
    // m_renderer.loadVertexShader("D:\\Documents\\gits\\project_marica_car_"
    //     "generator\\resources\\shaders\\meshvertexshader.vert");
    // auto fragmentShader =
    // m_renderer.loadFragmentShader("D:\\Documents\\gits\\project_marica_car_"
    // //     "generator\\resources\\shaders\\meshfragmentshader.frag");

    auto shaderProgram = m_renderer.getShaderProgram(
        ".\\resources\\shaders\\defaultvertexshader.vert",
        ".\\resources\\shaders\\defaultfragmentshader.frag");

    // auto vertexArray = std::make_shared<TriangleArray>();
    // std::vector<Vertex> vertices = {Vertex{-0.5f, -0.5f, 0.f, 0.f, 0.f, 0},
    //                                 Vertex{0.5f, -0.5f, 0.f, 1.f, 0.f, 0},
    //                                 Vertex{0.5f, 0.5f, 0.f, 1.f, 1.f, 0},
    //                                 Vertex{-0.5f, 0.5f, 0.f, 0.f, 1.f, 0},

    //                                 Vertex{0.5f, 0.5f, 0.f, 0.f, 0.f, 1},
    //                                 Vertex{1.0f, 0.5f, 0.f, 1.f, 0.f, 1},
    //                                 Vertex{1.0f, 1.0f, 0.f, 1.f, 1.f, 1},
    //                                 Vertex{0.5f, 1.0f, 0.f, 0.f, 1.f, 1}};

    // std::vector<Face> faces = {Face{{0, 1, 2}}, Face{{0, 3, 2}},
    //                            Face{{4, 5, 6}}, Face{{4, 7, 6}}};

    // auto vertexBuffer = std::make_shared<VertexBuffer>();
    // vertexBuffer->init(vertices);

    // auto elementBuffer = std::make_shared<ElementBuffer>();
    // elementBuffer->init(faces);

    // vertexArray->init(vertexBuffer, elementBuffer);

    // auto texture = std::make_shared<Texture>();
    // texture->init(
    //     wxImage("D:\\Documents\\gits\\project_marica_car_"
    //             "generator\\example\\MaricaFlatoutTex\\Texture\\common.png"));

    // Model model;

    // model.vertices = vertices;
    // model.faces = faces;
    // model.materials.emplace_back(
    //     "D:\\Documents\\gits\\project_marica_car_"
    //     "generator\\example\\MaricaFlatoutTex\\Texture\\common.png");
    // model.materials.emplace_back(
    //     "D:\\Documents\\gits\\project_marica_car_"
    //     "generator\\example\\MaricaFlatoutTex\\Texture\\skin1.png");

    // mesh = std::make_shared<Mesh>();
    // // mesh->init(model);

    // // mesh = std::make_shared<Mesh>();
    // mesh->init(Model::readPSK("D:\\Documents\\gits\\project_marica_car_"
    //                           "generator\\example\\MaricaFlatoutModels\\Skeleta"
    //                           "lMesh\\pm_speedevil_model01.psk"));

    // newNode = std::make_shared<SceneNode>();
    // newNode->addDrawable(mesh);
    // newNode->setLocation(Vec3f(2.0f, 0.0f, 0.0f));

    scene->init(shaderProgram);
    // scene->addNode(newNode);

    // batch = std::make_unique<Batch>(vertexArray, texture);

    // mesh = std::make_unique<Mesh>();
    // mesh->init(Model::readPSK("D:\\Documents\\gits\\project_marica_car_generator\\example\\MaricaFlatoutModels\\SkeletalMesh\\pm_speedevil_model01.psk"));

    // shaderProgram->bind();

    redrawTimer.SetOwner(this);
    redrawTimer.Start(1000.0f / 60.0f);

    updateTimer.SetOwner(this);
    updateTimer.Start(1000.0f / 60.0f);

    lastUpdate = std::chrono::steady_clock::now();
}

void OpenglView::onKeyDown(wxKeyEvent& event) {
    wxLogDebug("Key down");
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
    }
    else if (event.GetTimer().GetId() == redrawTimer.GetId()) {
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
