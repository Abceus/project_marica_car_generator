#pragma once
#include <map>
#include <memory>
#include <set>
#include <string>


#include "render_system/transform.h"
#include "utils/math/matrix.h"

class Bone : public std::enable_shared_from_this<Bone> {
public:
    void addChild(const std::shared_ptr<Bone>& newChild);
    void removeChild(const std::shared_ptr<Bone>& child);

    Transform& getTransform();
    Transform& getOrigin();

    Matrixf44 getGlobalMatrix() const;
    Matrixf44 getGlobalOriginMatrix() const;
private:
    void setParent(const std::weak_ptr<Bone>& newParent);
    void unsetParent();

    std::set<std::shared_ptr<Bone>> children;
    std::weak_ptr<Bone> parent;
    Transform transform;
    Transform origin;
};

class Skeletal {
public:
    std::weak_ptr<Bone> getBone(const std::string& name) const;
    void addBone(const std::string& key, const std::shared_ptr<Bone>& bone);
    std::set<std::string> getBoneNames() const;

private:
    std::map<std::string, std::shared_ptr<Bone>> bones;
};
