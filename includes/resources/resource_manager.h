#pragma once

#include <map>

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
    auto found = resources.find(key);
    if(found == resources.end())
    {
        resources.emplace(key, Value(key, args...));
        found = resources.find(key);
    }
    return found->second;
}

template<typename Key, typename Value, typename... Args>
Value ResourceManager::get( const Key& key, Args... args )
{
    static ResourcesHolder<Key, Value> manager;
    return manager.get(key, args...);
}
