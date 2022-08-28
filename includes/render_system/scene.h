#pragma once

#include "render_system/camera_manager.h"
#include "render_system/scene_node.h"
#include "render_system/shader_program.h"
#include "render_system/transform.h"
#include <memory>


class Scene {
public:
    Scene();
    ~Scene() = default;

    void init(const std::shared_ptr<ShaderProgram>& shaderProgram);

    void clear();

    void draw();
    void resizeScreen(int w, int h);

    std::shared_ptr<SceneNode>
    addNode(const std::shared_ptr<SceneNode>& newNode);

    std::shared_ptr<SceneNode> getActiveCamera() const;

private:
    void drawNode(const std::shared_ptr<SceneNode>& node,
                  const Transform& parentTransform = Transform());

    std::shared_ptr<SceneNode> m_rootNode;

    std::shared_ptr<ShaderProgram> m_shaderProgram;
    std::shared_ptr<ShaderProgram> m_defaultShaderProgram;
    glm::mat4 m_projection;

    std::unique_ptr<CameraManager> cameraManager;
};
