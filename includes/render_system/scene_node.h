#pragma once

#include "drawable.h"
#include "render_system/shader_program.h"
#include <glm/glm.hpp>
#include <memory>
#include <vector>


class SceneNode {
public:
    SceneNode();

    glm::vec3 getLocation() const;
    void setLocation(const glm::vec3& location);

    glm::vec3 getRotation() const;
    void setRotation(const glm::vec3& rotation);

    glm::vec3 getScale() const;
    void setScale(const glm::vec3& scale);
    void setScale(float scale);

    void setParent(SceneNode* parent);

    std::shared_ptr<SceneNode> addChild(const std::shared_ptr<SceneNode>& newChild);
    void removeChild(SceneNode* removeChild);

    std::shared_ptr<Drawable>
    addDrawable(const std::shared_ptr<Drawable>& newDrawable);
    void removeDrawable(Drawable* removeDrawable);

    bool isEmpty() const;
    void clear();

    std::vector<std::shared_ptr<SceneNode>>::const_iterator begin();
    std::vector<std::shared_ptr<SceneNode>>::const_iterator end();

    std::vector<std::shared_ptr<Drawable>>::const_iterator
    drawableBegin();
    std::vector<std::shared_ptr<Drawable>>::const_iterator drawableEnd();

    void setShaderProgram(const std::shared_ptr<ShaderProgram>& newProgram);
    std::shared_ptr<ShaderProgram> getShaderProgram() const;
private:
    glm::vec3 m_location;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;

    SceneNode* m_parent;
    std::vector<std::shared_ptr<SceneNode>> m_childrens;
    std::vector<std::shared_ptr<Drawable>> m_drawables;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
};
