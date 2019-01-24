#include "resource_manager.h"

template<typename Key, typename Value>
template<typename... Args>
Value ResourcesHolder<Key, Value>::get(Key key, Args... args)
{
    auto found = resources.find(key);
    if(found == resources.end())
    {
        resources.emplace(key, Value(key, args...));
        found = resources.find(key);
    }
    return found;
}

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

template<typename Key, typename Value, typename... Args>
Value ResourceManager::get( Key key, Args... args )
{
    static ResourcesHolder<Key, Value> manager;
    return manager.get(key, args...);
}
