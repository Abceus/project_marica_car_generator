#pragma once

#include <QMap>
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
    void remove( T* resource ) override;
private:
    QMap<QString, ResourcePointer<T>> m_resources;
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
}

template<typename T>
ResourcePointer<T> ResourceSubmanager<T>::get( const QString& path )
{
    auto fullPath = QFileInfo(path).absoluteFilePath();
    auto found = m_resources.find(fullPath);
    if(found == m_resources.end())
    {
        m_resources.insert(fullPath, ResourcePointer<T>( this, m_loader->load( path ) ) );
        found = m_resources.find(fullPath);
    }
    return found.value();
}

template<typename T>
void ResourceSubmanager<T>::remove( T* resource )
{
    m_loader->remove( resource );
    auto resourceKey = m_resources.key( ResourcePointer<T>( nullptr, resource ) );
    m_resources.erase( m_resources.find( resourceKey ) );
}
