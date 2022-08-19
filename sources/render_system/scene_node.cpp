#include "render_system/scene_node.h"
#include "render_system/drawable.h"

#include <algorithm>

SceneNode::SceneNode()
    : m_location(), m_rotation(), m_scale(1.0f, 1.0f, 1.0f), m_parent(nullptr),
      m_drawables(), m_shaderProgram(nullptr) {}

Vec3f SceneNode::getLocation() const {
    if (m_parent) {
        return m_location + m_parent->getLocation();
    }
    return m_location;
}

void SceneNode::setLocation(const Vec3f& location) {
    m_location = location;
}

Rotor3 SceneNode::getRotation() const {
    if (m_parent) {
        return m_rotation + m_parent->getRotation();
    }
    return m_rotation;
}

void SceneNode::setRotation(const Rotor3& rotation) {
    m_rotation = rotation;
}

Vec3f SceneNode::getScale() const {
    if (m_parent) {
        return m_scale * m_parent->getScale();
    }
    return m_scale;
}

void SceneNode::setScale(const Vec3f& scale) {
    m_scale = scale;
}

void SceneNode::setScale(float scale) {
    m_scale = {scale, scale, scale};
}

void SceneNode::setParent(SceneNode* parent) {
    if (m_parent == parent) {
        return;
    }

    if (m_parent) {
        m_parent->removeChild(this);
    }
    m_parent = parent;
}

std::shared_ptr<SceneNode>
SceneNode::addChild(const std::shared_ptr<SceneNode>& newChild) {
    if (std::find(std::begin(m_childrens), std::end(m_childrens), newChild) ==
        std::end(m_childrens)) {
        m_childrens.emplace_back(newChild);
        newChild->setParent(this);
    }
    return newChild;
}

void SceneNode::removeChild(SceneNode* removeChild) {
    auto found = std::find_if(std::begin(m_childrens), std::end(m_childrens),
                              [removeChild](const auto& child) {
                                  return child.get() == removeChild;
                              });
    if (found != m_childrens.end()) {
        m_childrens.erase(found);
        removeChild->setParent(nullptr);
    }
}

std::shared_ptr<Drawable>
SceneNode::addDrawable(const std::shared_ptr<Drawable>& newDrawable) {
    if (std::find(std::begin(m_drawables), std::end(m_drawables),
                  newDrawable) == std::end(m_drawables)) {
        m_drawables.emplace_back(newDrawable);
    }
    return newDrawable;
}

void SceneNode::removeDrawable(Drawable* removeDrawable) {
    auto found = std::find_if(m_drawables.begin(), m_drawables.end(),
                              [removeDrawable](const auto& drawable) {
                                  return drawable.get() == removeDrawable;
                              });
    if (found != m_drawables.end()) {
        m_drawables.erase(found);
    }
}

bool SceneNode::isEmpty() const {
    return m_childrens.empty();
}

void SceneNode::clear() {
    m_childrens.clear();
    m_drawables.clear();
}

std::vector<std::shared_ptr<SceneNode>>::const_iterator SceneNode::begin() {
    return m_childrens.begin();
}

std::vector<std::shared_ptr<SceneNode>>::const_iterator SceneNode::end() {
    return m_childrens.end();
}

std::vector<std::shared_ptr<Drawable>>::const_iterator
SceneNode::drawableBegin() {
    return m_drawables.begin();
}

std::vector<std::shared_ptr<Drawable>>::const_iterator
SceneNode::drawableEnd() {
    return m_drawables.end();
}

void SceneNode::setShaderProgram(
    const std::shared_ptr<ShaderProgram>& newProgram) {
    m_shaderProgram = newProgram;
}

std::shared_ptr<ShaderProgram> SceneNode::getShaderProgram() const {
    return m_shaderProgram;
}
