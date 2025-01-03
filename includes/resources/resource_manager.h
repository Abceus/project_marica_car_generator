#pragma once

#include "utils/concepts/can_call.h"
#include <functional>
#include <map>
#include <memory>
#include <string>

template <typename Key, typename Value, typename... Args>
class ResourcesHolder {
public:
    ResourcesHolder() = default;

    template <CanCall<Key, Args...> CustomFactory>
    ResourcesHolder(const CustomFactory& customFactory_);

    template <typename... GetArgs>
    std::shared_ptr<Value> get(const Key& key, GetArgs&&... args);

    void setDefaultResource(const std::shared_ptr<Value>& newDefaultResource);
    std::shared_ptr<Value> getDefaultResource() const;

    template <CanCall<Key, Args...> CustomFactory>
    void setCustomFactory(const CustomFactory& customFactory_);

private:
    std::map<Key, std::weak_ptr<Value>> resources;
    std::shared_ptr<Value> defaultResource;
    std::function<std::shared_ptr<Value>(Key, Args...)> customFactory;
};

template <typename Value>
using FileResourceHolder = ResourcesHolder<std::string, Value>;

class ResourceManager {
public:
    static ResourceManager& Instance();
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // FileResourceHolder<wxImage>& getImageManager();

private:
    ResourceManager();
    ~ResourceManager() = default;

    // FileResourceHolder<wxImage> imageManager;
};

template <typename Key, typename Value, typename... Args>
template <CanCall<Key, Args...> CustomFactory>
ResourcesHolder<Key, Value, Args...>::ResourcesHolder(
    const CustomFactory& customFactory_)
    : customFactory(customFactory_) {}

template <typename Key, typename Value, typename... Args>
template <typename... GetArgs>
std::shared_ptr<Value>
ResourcesHolder<Key, Value, Args...>::get(const Key& key, GetArgs&&... args) {
    auto found = resources.find(key);
    if (found == resources.end() || !found->second.lock()) {
        std::shared_ptr<Value> result;
        if (customFactory) {
            result = customFactory(key, args...);
        } else {
            result =
                std::shared_ptr<Value>(new Value(key, std::forward(args)...));
        }
        resources.emplace(key, result);
        return result;
    }
    return found->second.lock();
}

template <typename Key, typename Value, typename... Args>
void ResourcesHolder<Key, Value, Args...>::setDefaultResource(
    const std::shared_ptr<Value>& newDefaultResource) {
    defaultResource = newDefaultResource;
}

template <typename Key, typename Value, typename... Args>
std::shared_ptr<Value>
ResourcesHolder<Key, Value, Args...>::getDefaultResource() const {
    return defaultResource;
}

template <typename Key, typename Value, typename... Args>
template <CanCall<Key, Args...> CustomFactory>
void ResourcesHolder<Key, Value, Args...>::setCustomFactory(
    const CustomFactory& customFactory_) {
    customFactory = customFactory_;
}