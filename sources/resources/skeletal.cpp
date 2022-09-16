#include "resources/skeletal.h"
#include <string>

void Bone::addChild(const std::shared_ptr<Bone>& newChild) {
    if (!children.contains(newChild)) {
        children.insert(newChild);
        newChild->setParent(weak_from_this());
    }
}

void Bone::removeChild(const std::shared_ptr<Bone>& child) {
    if (children.contains(child)) {
        children.erase(child);
        child->unsetParent();
    }
}

Transform& Bone::getTransform() {
    return transform;
}

Transform& Bone::getOrigin() {
    return origin;
}

Matrixf44 Bone::getGlobalMatrix() const {
    // if (auto lockedParent = parent.lock()) {
    //     return lockedParent->getGlobalMatrix() * transform.getMatrix();
    // }
    return transform.getLocationMatrix() * transform.getRotationMatrix() * transform.getScaleMatrix();
}

Matrixf44 Bone::getGlobalOriginMatrix() const {
    if (auto lockedParent = parent.lock()) {
        return lockedParent->getGlobalOriginMatrix() * origin.getMatrix();
    }
    return origin.getLocationMatrix() * origin.getRotationMatrix() * origin.getScaleMatrix();
}

void Bone::setParent(const std::weak_ptr<Bone>& newParent) {
    if (parent.lock() != newParent.lock()) {
        parent = newParent;
        newParent.lock()->addChild(shared_from_this());
    }
}

void Bone::unsetParent() {
    auto oldParent = parent;
    parent.reset();
    oldParent.lock()->removeChild(shared_from_this());
}

std::weak_ptr<Bone> Skeletal::getBone(const std::string& name) const {
    auto found = bones.find(name);
    if (found != std::end(bones)) {
        return found->second;
    }
    return std::weak_ptr<Bone>();
}

void Skeletal::addBone(const std::string& key,
                       const std::shared_ptr<Bone>& bone) {
    assert(bones.find(key) == std::end(bones));
    bones.emplace(key, bone);
}

std::set<std::string> Skeletal::getBoneNames() const {
    std::set<std::string> result;
    for(const auto& bone: bones) {
        result.insert(bone.first);
    }
    return result;
}
