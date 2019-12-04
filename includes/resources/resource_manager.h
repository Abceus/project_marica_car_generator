#pragma once

#include <map>
#include <QFileInfo>

template<typename Key, typename Value>
class ResourcesHolder
{
public:
    template<typename... Args>
    Value get( const Key& key, Args... args );
private:
    std::map<Key, Value> resources;
};

class ResourceManager
{
public:
    static ResourceManager& Instance();
    ResourceManager( ResourceManager const& ) = delete;
    ResourceManager& operator= ( ResourceManager const& ) = delete;
    template<typename Key, typename Value, typename... Args>
    Value get( const Key& key, Args... args );
private:
    ResourceManager() = default;
    ~ResourceManager() = default;
};

template<typename Key, typename Value>
template<typename... Args>
Value ResourcesHolder<Key, Value>::get( const Key& key, Args... args )
{
    auto fullPath = QFileInfo(key).absoluteFilePath();
    auto found = resources.find(fullPath);
    if(found == resources.end())
    {
        resources.emplace(fullPath, Value(fullPath, args...));
        found = resources.find(fullPath);
    }
    return found->second;
}

template<typename Key, typename Value, typename... Args>
Value ResourceManager::get( const Key& key, Args... args )
{
    static ResourcesHolder<Key, Value> manager;
    return manager.get(key, args...);
}
