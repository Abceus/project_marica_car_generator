#pragma once

#include <map>
#include <QSharedPointer>

template<typename Key, typename Value>
class ResourcesHolder
{
public:
    template<typename... Args>
    QSharedPointer<Value> get( const Key& key, Args&&... args );
private:
    std::map<Key, QWeakPointer<Value>> resources;
};

class ResourceManager
{
public:
    static ResourceManager& Instance();
    ResourceManager( const ResourceManager& ) = delete;
    ResourceManager& operator= ( const ResourceManager& ) = delete;

    template<typename Key, typename Value, typename... Args>
    QSharedPointer<Value> get( const Key& key, Args&&... args );
private:
    ResourceManager() = default;
    ~ResourceManager() = default;
};

template<typename Key, typename Value>
template<typename... Args>
QSharedPointer<Value> ResourcesHolder<Key, Value>::get( const Key& key, Args&&... args )
{
    auto found = resources.find(key);
    if(found == resources.end() || !found->second.lock())
    {
        auto result = QSharedPointer<Value>(new Value(key, std::forward(args)...));
        resources.emplace(key, result);
        return result;
    }
    return found->second.lock();
}

template<typename Key, typename Value, typename... Args>
QSharedPointer<Value> ResourceManager::get( const Key& key, Args&&... args )
{
    static ResourcesHolder<Key, Value> manager;
    return manager.get(key, std::forward(args)...);
}
