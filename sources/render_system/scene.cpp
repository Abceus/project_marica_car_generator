#include "render_system/scene.h"
#include "glm/fwd.hpp"
#include "glm/trigonometric.hpp"
#include "render_system/camera.h"
#include "render_system/camera_manager.h"
#include <memory>

Scene::Scene()
    : m_rootNode(std::make_shared<SceneNode>()),
      cameraManager(std::make_unique<CameraManager>()) {
    cameraManager->addCamera("main", std::make_shared<Camera>());
    cameraManager->setActiveCamera("main");
    cameraManager->getActiveCamera()->setRotation(glm::vec3(0.0f, -90.0f, 0.0f));
}

void Scene::init(const std::shared_ptr<ShaderProgram>& shaderProgram) {
    m_defaultShaderProgram = shaderProgram;
}

void Scene::clear() {
    cameraManager->getActiveCamera()->setLocation(glm::vec3());
    cameraManager->getActiveCamera()->setRotation(glm::vec3());
    cameraManager->getActiveCamera()->setScale(1.0f);

    m_rootNode->clear();
}

void Scene::draw() {
    drawNode(m_rootNode);
    m_shaderProgram->unbind();
}

void Scene::resizeScreen(int w, int h) {
    // Calculate aspect ratio
    const float aspect = static_cast<float>(w) / h;
    const float zNear = 1.0f, zFar = 10000.f, fov = 90.f;

    // Reset projection
    m_projection = glm::perspective(glm::radians(fov), aspect, zNear, zFar);
    // m_projection = glm::mat4(1);
}

std::shared_ptr<SceneNode>
Scene::addNode(const std::shared_ptr<SceneNode>& newNode) {
    m_rootNode->addChild(newNode);
    return newNode;
}

void Scene::drawNode(const std::shared_ptr<SceneNode>& node) {
    bool shaderChanged = true;
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

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, -2.0f));
    model = glm::rotate(model, glm::radians(node->getRotation().x), glm::vec3(1, 0, 0));
    model = glm::rotate(model, glm::radians(node->getRotation().y), glm::vec3(0, 1, 0));
    model = glm::rotate(model, glm::radians(node->getRotation().z), glm::vec3(0, 0, 1));
    model = glm::scale(model, node->getScale());

    m_shaderProgram->setUniform("model", model);

    for (auto i = node->drawableBegin(); i != node->drawableEnd(); i++) {
        (*i)->draw();
    }

    for (const auto& childNode : *node) {
        //        drawNode( childNode, parentMatrix, f, ef );
        drawNode(childNode);
    }
}

std::shared_ptr<SceneNode> Scene::getActiveCamera() const {
    return cameraManager->getActiveCamera();
}