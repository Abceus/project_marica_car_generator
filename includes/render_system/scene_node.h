#pragma once

#include "drawable.h"
#include "render_system/shader_program.h"
#include "utils/math/vec3.h"
#include "utils/math/rot3.h"
#include <memory>
#include <vector>


class SceneNode {
public:
    SceneNode();

    Vec3f getLocation() const;
    void setLocation(const Vec3f& location);

    Rotor3 getRotation() const;
    void setRotation(const Rotor3& rotation);

    Vec3f getScale() const;
    void setScale(const Vec3f& scale);
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
    Vec3f m_location;
    Rotor3 m_rotation;
    Vec3f m_scale;

    SceneNode* m_parent;
    std::vector<std::shared_ptr<SceneNode>> m_childrens;
    std::vector<std::shared_ptr<Drawable>> m_drawables;
    std::shared_ptr<ShaderProgram> m_shaderProgram;
};
