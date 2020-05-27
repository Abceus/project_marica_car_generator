#pragma once

#include <map>
#include <QDebug>
#include <QString>
#include <QScopedPointer>
#include <QFileInfo>
#include "resources/resource_pointer.h"
#include "resources/loaders/loader.h"

template<typename T>
class ResourceSubmanager : public IResourceMaster<T>
{
public:
    ResourceSubmanager( ILoader<T>* loader );
    ~ResourceSubmanager();
    ResourcePointer<T> get( const QString& path );
    void remove( ResourcePointer<T> resource ) override;
private:
    std::map<QString, ResourcePointer<T>> m_resources;
    ILoader<T>* m_loader;
};

template<typename T>
ResourceSubmanager<T>::ResourceSubmanager( ILoader<T>* loader )
    : m_resources()
    , m_loader( loader )
{
}

template<typename T>
ResourceSubmanager<T>::~ResourceSubmanager<T>()
{
    delete m_loader;
    for( auto& resource: m_resources )
    {
        resource.second.unsetMaster();
    }
}

template<typename T>
ResourcePointer<T> ResourceSubmanager<T>::get( const QString& path )
{
    auto fullPath = QFileInfo(path).absoluteFilePath();
    auto found = m_resources.find(fullPath);
    if(found == m_resources.end())
    {
        auto resource = m_loader->load( path );
        if(resource)
        {
            m_resources.emplace(fullPath, ResourcePointer<T>( this, m_loader->load( path ), path ) );
            found = m_resources.find(fullPath);
        }
        else
        {
            return ResourcePointer<T>();
        }
    }
    return found->second;
}

template<typename T>
void ResourceSubmanager<T>::remove( ResourcePointer<T> resource )
{
    if( !resource )
    {
        return;
    }
    m_loader->remove( resource.get() );
    auto found = m_resources.find( QFileInfo( resource.getPath() ).absoluteFilePath() );
    if(found != m_resources.end())
    {
        m_resources.erase( found );
    }
}
