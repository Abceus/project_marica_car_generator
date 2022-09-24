#include "render_system/scene.h"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "render_system/camera.h"
#include "render_system/camera_manager.h"
#include "utils/math/matrix.h"
#include "utils/math/rot3.h"
#include "utils/math/vec3.h"
#include <memory>

Scene::Scene()
    : m_rootNode(std::make_shared<SceneNode>()),
      cameraManager(std::make_unique<CameraManager>()) {
    cameraManager->addCamera("main", std::make_shared<Camera>());
    cameraManager->setActiveCamera("main");
}

void Scene::init(const std::shared_ptr<ShaderProgram>& shaderProgram) {
    m_defaultShaderProgram = shaderProgram;
}

void Scene::clear() {
    cameraManager->getActiveCamera()->setLocation({});
    cameraManager->getActiveCamera()->setRotation({});
    cameraManager->getActiveCamera()->setScale(1.0f);

    m_rootNode->clear();
}

void Scene::draw() {
    std::list<std::shared_ptr<SceneNode>> overlayList;
    drawNodeRecurse(m_rootNode, overlayList);
    glDisable(GL_DEPTH_TEST);
    for (const auto& overlay : overlayList) {
        drawNode(overlay);
    }
    glEnable(GL_DEPTH_TEST);
    m_shaderProgram->unbind();
    m_shaderProgram = nullptr;
}

void Scene::resizeScreen(int w, int h) {
    // Calculate aspect ratio
    const float aspect = static_cast<float>(w) / h;
    const float zNear = 1.0f, zFar = 10000.f, fov = 90.f;

    // Reset projection
    m_projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
}

std::shared_ptr<SceneNode>
Scene::addNode(const std::shared_ptr<SceneNode>& newNode) {
    m_rootNode->addChild(newNode);
    return newNode;
}

void Scene::drawNode(const std::shared_ptr<SceneNode>& node) {
    bool shaderChanged = false;
    if (node->getShaderProgram()) {
        if (m_shaderProgram != node->getShaderProgram()) {
            if (m_shaderProgram) {
                m_shaderProgram->unbind();
            }
            m_shaderProgram = node->getShaderProgram();
            shaderChanged = true;
        }
    } else if (m_shaderProgram != m_defaultShaderProgram) {
        if (m_shaderProgram) {
            m_shaderProgram->unbind();
        }
        m_shaderProgram = m_defaultShaderProgram;
        shaderChanged = true;
    }

    if (shaderChanged) {
        m_shaderProgram->bind();

        auto view = cameraManager->getActiveCamera()->getProjectionMatrix();

        m_shaderProgram->setUniform("view", view);
        m_shaderProgram->setUniform("projection", m_projection);
    }

    Matrixf44 model(1.0f);
    model = model.translate(model, node->getGlobalLocation());
    model = model.rotate(model, node->getGlobalRotation());
    model = model.scale(model, node->getGlobalScale());
    m_shaderProgram->setUniform("model", model.getGLMat());

    for (auto i = node->drawableBegin(); i != node->drawableEnd(); i++) {
        (*i)->draw(m_shaderProgram.get());
    }
}

void Scene::drawNodeRecurse(const std::shared_ptr<SceneNode>& node,
                     std::list<std::shared_ptr<SceneNode>>& overlayList) {
    drawNode(node);

    for (const auto& childNode : *node) {
        if(childNode->isOverlay()) {
            overlayList.emplace_back(childNode);
        } else {
            drawNodeRecurse(childNode, overlayList);
        }
    }
}

std::shared_ptr<SceneNode> Scene::getActiveCamera() const {
    return cameraManager->getActiveCamera();
}