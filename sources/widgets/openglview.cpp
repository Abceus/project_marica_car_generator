#include "widgets/openglview.h"
#include "render_system/element_buffer.h"
#include "render_system/mesh.h"
#include "render_system/scene_node.h"
#include "render_system/vertex_array.h"
#include "utils/math/utils.h"
#include "wx/dcclient.h"
#include "wx/event.h"
#include "wx/image.h"
#include "wx/log.h"
#include "wx/utils.h"
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

    Bind(wxEVT_MOTION, &OpenglView::onMouseEvent, this);
    Bind(wxEVT_ENTER_WINDOW, &OpenglView::onMouseFocusEvent, this);
    Bind(wxEVT_LEAVE_WINDOW, &OpenglView::onMouseFocusEvent, this);

    SetWindowStyleFlag(GetWindowStyleFlag() | wxWANTS_CHARS);

    scene = std::make_unique<Scene>();
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
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
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
    for (auto i = newNode->drawableBegin(); i != newNode->drawableEnd(); i++) {
        // (*i)->draw();
    }
    scene->draw();

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

    std::ifstream fragmentStream(
        "D:\\Documents\\gits\\project_marica_car_"
        "generator\\resources\\shaders\\meshfragmentshader.frag");
    std::stringstream fragmentBuffer;
    fragmentBuffer << fragmentStream.rdbuf();

    std::ifstream vertexStream(
        "D:\\Documents\\gits\\project_marica_car_"
        "generator\\resources\\shaders\\meshvertexshader.vert");
    std::stringstream vertexShaderBuffer;
    vertexShaderBuffer << vertexStream.rdbuf();

    shaderProgram = std::make_shared<ShaderProgram>();
    shaderProgram->init(vertexShaderBuffer.str(), fragmentBuffer.str());

    auto vertexArray = std::make_shared<VertexArray>();
    std::vector<Vertex> vertices = {Vertex{-0.5f, -0.5f, 0.f, 0.f, 0.f, 0},
                                    Vertex{0.5f, -0.5f, 0.f, 1.f, 0.f, 0},
                                    Vertex{0.5f, 0.5f, 0.f, 1.f, 1.f, 0},
                                    Vertex{-0.5f, 0.5f, 0.f, 0.f, 1.f, 0},

                                    Vertex{0.5f, 0.5f, 0.f, 0.f, 0.f, 1},
                                    Vertex{1.0f, 0.5f, 0.f, 1.f, 0.f, 1},
                                    Vertex{1.0f, 1.0f, 0.f, 1.f, 1.f, 1},
                                    Vertex{0.5f, 1.0f, 0.f, 0.f, 1.f, 1}};

    std::vector<Face> faces = {Face{{0, 1, 2}}, Face{{0, 3, 2}},
                               Face{{4, 5, 6}}, Face{{4, 7, 6}}};

    // auto vertexBuffer = std::make_shared<VertexBuffer>();
    // vertexBuffer->init(vertices);

    // auto elementBuffer = std::make_shared<ElementBuffer>();
    // elementBuffer->init(faces);

    // vertexArray->init(vertexBuffer, elementBuffer);

    // auto texture = std::make_shared<Texture>();
    // texture->init(
    //     wxImage("D:\\Documents\\gits\\project_marica_car_"
    //             "generator\\example\\MaricaFlatoutTex\\Texture\\common.png"));

    Model model;

    model.vertices = vertices;
    model.faces = faces;
    model.materials.emplace_back(
        "D:\\Documents\\gits\\project_marica_car_"
        "generator\\example\\MaricaFlatoutTex\\Texture\\common.png");
    model.materials.emplace_back(
        "D:\\Documents\\gits\\project_marica_car_"
        "generator\\example\\MaricaFlatoutTex\\Texture\\skin1.png");

    mesh = std::make_shared<Mesh>();
    // mesh->init(model);

    // mesh = std::make_shared<Mesh>();
    mesh->init(Model::readPSK("D:\\Documents\\gits\\project_marica_car_"
                              "generator\\example\\MaricaFlatoutModels\\Skeleta"
                              "lMesh\\pm_speedevil_model01.psk"));

    newNode = std::make_shared<SceneNode>();
    newNode->addDrawable(mesh);
    newNode->setLocation(Vec3f(2.0f, 0.0f, 0.0f));

    scene->init(shaderProgram);
    scene->addNode(newNode);

    // batch = std::make_unique<Batch>(vertexArray, texture);

    // mesh = std::make_unique<Mesh>();
    // mesh->init(Model::readPSK("D:\\Documents\\gits\\project_marica_car_generator\\example\\MaricaFlatoutModels\\SkeletalMesh\\pm_speedevil_model01.psk"));

    // shaderProgram->bind();

    redrawTimer.SetOwner(this);
    redrawTimer.Start(1000.0f / 60.0f);
}

void OpenglView::onKeyDown(wxKeyEvent& event) {
    wxLogDebug("Key down");
}

void OpenglView::onKeyUp(wxKeyEvent& event) {
    wxLogDebug("Key up");
}

void OpenglView::onTimer(wxTimerEvent& event) {
    Refresh();
}

void OpenglView::onMouseEvent(wxMouseEvent& event) {
    const auto NoneState = 0b000;
    const auto LeftButtonState = 0b001;
    const auto RightButtonState = 0b010;
    const auto BothButtonState = LeftButtonState | RightButtonState;

    const auto ShiftState = 0b100;
    const auto ShiftLeftButtonState = ShiftState | LeftButtonState;
    const auto ShiftRightButtonState = ShiftState | RightButtonState;
    const auto ShiftBothButtonState = ShiftState | BothButtonState;

    auto controlState = NoneState;
    if (wxGetKeyState(WXK_SHIFT)) {
        controlState |= ShiftState;
    }
    if (event.LeftIsDown()) {
        controlState |= LeftButtonState;
    }
    if (event.RightIsDown()) {
        controlState |= RightButtonState;
    }
    if (controlState != NoneState) {
        SetCursor(wxCursor(wxImage(1, 1)));
        if (event.Dragging()) {
            auto currentCamera = scene->getActiveCamera();
            auto currentCameraLocation = currentCamera->getLocation();
            auto currentCameraRotation = currentCamera->getRotation();
            auto delta = glm::vec2();
            auto currentMousePosition = glm::vec2{event.GetX(), event.GetY()};
            if (prevMousePosition) {
                delta = currentMousePosition - prevMousePosition.value();
            } else {
                prevMousePosition = currentMousePosition;
            }
            WarpPointer(prevMousePosition->x, prevMousePosition->y);
            auto moveSpeed = 5.0f;
            auto rotateSpeed = 1.0f;

            switch (controlState) {
            case ShiftLeftButtonState: {
                currentCameraLocation.setX(currentCameraLocation.getX() +
                                           delta.x * moveSpeed);
                break;
            }
            case ShiftRightButtonState: {
                currentCameraLocation.setY(currentCameraLocation.getY() +
                                           delta.x * moveSpeed);
                break;
            }
            case ShiftBothButtonState: {
                currentCameraLocation.setZ(currentCameraLocation.getZ() +
                                           delta.x * moveSpeed);
                break;
            }
            case LeftButtonState: {
                currentCameraRotation.setRoll(
                    currentCameraRotation.getRoll() +
                    Angle::fromDegrees(delta.x * rotateSpeed));
                currentCameraLocation =
                    currentCameraLocation +
                    rotate(Vec3f(-delta.y * moveSpeed, 0.0f, 0.0f),
                           currentCameraRotation);
                break;
            }
            case RightButtonState: {
                currentCameraRotation.setRoll(
                    currentCameraRotation.getRoll() +
                    Angle::fromDegrees(delta.x * rotateSpeed));
                currentCameraRotation.setYaw(
                    currentCameraRotation.getYaw() +
                    Angle::fromDegrees(delta.y * rotateSpeed));
                break;
            }
            case BothButtonState: {
                currentCameraLocation = currentCameraLocation +
                                        rotate(Vec3f(0.0f, delta.x * moveSpeed,
                                                     -delta.y * moveSpeed),
                                               currentCameraRotation);
                break;
            }
            }
            currentCamera->setLocation(currentCameraLocation);
            currentCamera->setRotation(currentCameraRotation);
        }
    } else {
        prevMousePosition = std::nullopt;
        SetCursor(*wxCROSS_CURSOR);
    }
}

void OpenglView::onMouseFocusEvent(wxMouseEvent& event) {
    if (event.Leaving()) {
        SetCursor(*wxSTANDARD_CURSOR);
    } else if (event.Entering()) {
        SetCursor(*wxCROSS_CURSOR);
    }
}