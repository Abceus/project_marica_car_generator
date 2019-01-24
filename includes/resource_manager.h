#pragma once

#include <map>

template<typename Key, typename Value>
class ResourcesHolder
{
public:
    template<typename... Args>
    Value get(Key key, Args... args);
private:
    std::map<Key, Value> resources;
};

class ResourceManager
{
public:
    static ResourceManager& Instance();
    ResourceManager(ResourceManager const&) = delete;
    ResourceManager& operator= (ResourceManager const&) = delete;
    template<typename Key, typename Value, typename... Args>
    Value get(Key key, Args... args);
private:
    ResourceManager() = default;
    ~ResourceManager() = default;
};