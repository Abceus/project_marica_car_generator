#include "render_system/scene_node.h"
#include "render_system/drawable.h"
#include "render_system/transform.h"
#include "utils/math/matrix.h"
#include "utils/math/utils.h"

#include <algorithm>

SceneNode::SceneNode()
    : transform(), m_parent(nullptr), m_drawables(), m_shaderProgram(nullptr) {}

Vec3f SceneNode::getLocation() const {
    return transform.getLocation();
}

Vec3f SceneNode::getGlobalLocation() const {
    if (m_parent) {
        return Matrixf44::apply(m_parent->getGlobalTransformMatrix(),
                                getLocation());
    }
    return getLocation();
}

void SceneNode::setLocation(const Vec3f& location) {
    transform.setLocation(location);
}

Quaternion SceneNode::getRotation() const {
    return transform.getRotation();
}

Quaternion SceneNode::getGlobalRotation() const {
    if (m_parent) {
        auto matrix = m_parent->getGlobalRotationMatrix() * getRotationMatrix();
        return getQuaternionFromMatrix(matrix);
    }
    return getRotation();
}

Matrixf44 SceneNode::getRotationMatrix() const {
    return transform.getRotationMatrix();
}

Matrixf44 SceneNode::getGlobalRotationMatrix() const {
    if (m_parent) {
        return m_parent->getGlobalRotationMatrix() * getRotationMatrix();
    }
    return getRotationMatrix();
}

void SceneNode::setRotation(const Quaternion& rotation) {
    transform.setRotation(rotation);
}

Vec3f SceneNode::getScale() const {
    return transform.getScale();
}

Vec3f SceneNode::getGlobalScale() const {
    if (m_parent) {
        auto tm = getGlobalTransformMatrix();
        return {Vec3f(tm[0][0], tm[1][0], tm[2][0]).length(),
                Vec3f(tm[0][1], tm[1][1], tm[2][1]).length(),
                Vec3f(tm[0][2], tm[1][2], tm[2][2]).length()};
    }
    return getScale();
}

void SceneNode::setScale(const Vec3f& scale) {
    transform.setScale(scale);
}

void SceneNode::setScale(float scale) {
    transform.setScale(scale);
}

Matrixf44 SceneNode::getTransformMatrix() const {
    return transform.getMatrix();
}

Matrixf44 SceneNode::getGlobalTransformMatrix() const {
    if (m_parent) {
        return m_parent->getGlobalTransformMatrix() * getTransformMatrix();
    }
    return getTransformMatrix();
}

Transform SceneNode::getTransform() const {
    return transform;
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
