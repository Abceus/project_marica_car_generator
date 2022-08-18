// TODO: remove qt
#pragma once

#include <map>
#include <functional>
#include <QSharedPointer>
#include "utils/concepts/can_call.h"

template<typename Key, typename Value, typename... Args>
class ResourcesHolder
{
public:
    ResourcesHolder() = default;

    template<CanCall<Key, Args...> CustomFactory>
    ResourcesHolder(const CustomFactory& customFactory_);

    template<typename... GetArgs>
    QSharedPointer<Value> get( const Key& key, GetArgs&&... args );

    void setDefaultResource(const QSharedPointer<Value>& newDefaultResource);
    QSharedPointer<Value> getDefaultResource() const;

    template<CanCall<Key, Args...> CustomFactory>
    void setCustomFactory(const CustomFactory& customFactory_);
private:
    std::map<Key, QWeakPointer<Value>> resources;
    QSharedPointer<Value> defaultResource;
    std::function<QSharedPointer<Value>(Key, Args...)> customFactory;
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

template<typename Key, typename Value, typename... Args>
template<CanCall<Key, Args...> CustomFactory>
ResourcesHolder<Key, Value, Args...>::ResourcesHolder(const CustomFactory& customFactory_) : customFactory(customFactory_)
{
}

template<typename Key, typename Value, typename... Args>
template<typename... GetArgs>
QSharedPointer<Value> ResourcesHolder<Key, Value, Args...>::get( const Key& key, GetArgs&&... args )
{
    auto found = resources.find(key);
    if(found == resources.end() || !found->second.lock())
    {
        QSharedPointer<Value> result;
        if(customFactory) 
        {
            result = customFactory(key, args...);
        }
        else 
        {
            result = QSharedPointer<Value>(new Value(key, std::forward(args)...));
        }
        resources.emplace(key, result);
        return result;
    }
    return found->second.lock();
}

template<typename Key, typename Value, typename... Args>
void ResourcesHolder<Key, Value, Args...>::setDefaultResource(const QSharedPointer<Value>& newDefaultResource) 
{
    defaultResource = newDefaultResource;
}

template<typename Key, typename Value, typename... Args>
QSharedPointer<Value> ResourcesHolder<Key, Value, Args...>::getDefaultResource() const 
{
    return defaultResource;
}

template<typename Key, typename Value, typename... Args>
template<CanCall<Key, Args...> CustomFactory>
void ResourcesHolder<Key, Value, Args...>::setCustomFactory(const CustomFactory& customFactory_)
{
    customFactory = customFactory_;
}