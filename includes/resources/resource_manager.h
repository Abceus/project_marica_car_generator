#pragma once

#include <map>
#include <QSharedPointer>

template<typename Key, typename Value>
class ResourcesHolder
{
public:
    template<typename... Args>
    QSharedPointer<Value> get( const Key& key, Args&&... args );

    void setDefaultResource(const QSharedPointer<Value>& newDefaultResource);
    QSharedPointer<Value> getDefaultResource() const;
private:
    std::map<Key, QWeakPointer<Value>> resources;
    QSharedPointer<Value> defaultResource;
};

template<typename Value>
using FileResourceHolder = ResourcesHolder<QString, Value>;

class ResourceManager
{
public:
    static ResourceManager& Instance();
    ResourceManager( const ResourceManager& ) = delete;
    ResourceManager& operator= ( const ResourceManager& ) = delete;

    FileResourceHolder<QImage>& getImageManager();
private:
    ResourceManager();
    ~ResourceManager() = default;

    FileResourceHolder<QImage> imageManager;
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

template<typename Key, typename Value>
void ResourcesHolder<Key, Value>::setDefaultResource(const QSharedPointer<Value>& newDefaultResource) 
{
    defaultResource = newDefaultResource;
}

template<typename Key, typename Value>
QSharedPointer<Value> ResourcesHolder<Key, Value>::getDefaultResource() const 
{
    return defaultResource;
}