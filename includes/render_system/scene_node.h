#pragma once

#include "drawable.h"
#include "render_system/shader_program.h"
#include "render_system/transform.h"
#include "utils/math/matrix.h"
#include "utils/math/vec3.h"
#include "utils/math/quaternion.h"
#include <memory>
#include <vector>


class SceneNode {
public:
    SceneNode();

    Vec3f getLocation() const;
    Vec3f getGlobalLocation() const;
    Matrixf44 getLocationMatrix() const;
    Matrixf44 getGlobalLocationMatrix() const;
    void setLocation(const Vec3f& location);

    Quaternion getRotation() const;
    Quaternion getGlobalRotation() const;
    Matrixf44 getRotationMatrix() const;
    Matrixf44 getGlobalRotationMatrix() const;
    void setRotation(const Quaternion& rotation);

    Vec3f getScale() const;
    Vec3f getGlobalScale() const;
    Matrixf44 getScaleMatrix() const;
    Matrixf44 getGlobalScaleMatrix() const;
    void setScale(const Vec3f& scale);
    void setScale(float scale);

    Matrixf44 getTransformMatrix() const;
    Matrixf44 getGlobalTransformMatrix() const;

    Transform getTransform() const;

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
    
    void setOverlay(bool newValue);
    bool isOverlay() const;
private:
    Transform transform;

    SceneNode* m_parent;
    std::vector<std::shared_ptr<SceneNode>> m_childrens;
    std::vector<std::shared_ptr<Drawable>> m_drawables;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
    bool overlay = false;
};
