#pragma once

#include <stddef.h>
#include <QString>
#include <optional>

template<typename T>
class ResourcePointer;

template<typename T>
class IResourceMaster
{
public:
    virtual void remove( ResourcePointer<T> resource ) = 0;
};

class Counter
{
public:
    Counter(bool hasMaster);
    Counter operator++(int);
    Counter operator--(int);
    bool operator==(int other) const;
    bool needRemove() const;
    void unsetMaster();
private:
    bool m_createFinished;
    size_t m_count;
    bool m_hasMaster;
};

template<typename T>
class ResourcePointer
{
public:
    ResourcePointer( IResourceMaster<T>* masterManager=nullptr, T* resource=nullptr, QString path="" );
    ~ResourcePointer();
    ResourcePointer( const ResourcePointer<T>& copy );
    ResourcePointer( ResourcePointer<T>&& copy );
    ResourcePointer<T>& operator= ( ResourcePointer<T> copy );
//    ResourcePointer<T>& operator= ( ResourcePointer<T>&& copy );
    friend void swap(ResourcePointer<T>& first, ResourcePointer<T>& second);
    T* operator->();
    T* operator->() const;
    T *get();
    T *get() const;
    QString getPath() const;
    void unsetMaster();
    bool operator== (ResourcePointer<T> rhs) const;
    bool operator== (T* rhs) const;
    friend bool operator== (T* lhs, ResourcePointer<T> rhs);
    operator bool() const;
private:
    struct MasterWrapper
    {
         IResourceMaster<T>* value;
    };
    T* m_resource;
    MasterWrapper* m_masterManager;
    Counter* m_counter;
    QString m_path;
};

template<typename T>
bool operator== (T* lhs, ResourcePointer<T> rhs)
{
    return lhs == rhs.m_resource;
}

template<typename T>
ResourcePointer<T>::ResourcePointer(IResourceMaster<T> *masterManager, T *resource, QString path)
    : m_resource( resource )
    , m_masterManager( resource ? new MasterWrapper{ masterManager } : nullptr )
    , m_counter( resource ? new Counter( masterManager != nullptr ) : nullptr )
    , m_path( std::move( path ) )
{

}

template<typename T>
void swap(ResourcePointer<T>& first, ResourcePointer<T>& second)
{
    using std::swap;

    swap(first.m_resource, second.m_resource);
    swap(first.m_masterManager, second.m_masterManager);
    swap(first.m_counter, second.m_counter);
    swap(first.m_path, second.m_path);
}

template<typename T>
T *ResourcePointer<T>::operator->()
{
    return m_resource;
}

template<typename T>
T *ResourcePointer<T>::operator->() const
{
    return m_resource;
}

template<typename T>
T *ResourcePointer<T>::get()
{
    return m_resource;
}

template<typename T>
T *ResourcePointer<T>::get() const
{
    return m_resource;
}

template<typename T>
QString ResourcePointer<T>::getPath() const
{
    return m_path;
}

template<typename T>
void ResourcePointer<T>::unsetMaster()
{
    if(m_resource)
    {
        m_masterManager->value = nullptr;
    }
    m_counter->unsetMaster();
}

template<typename T>
bool ResourcePointer<T>::operator==(T* rhs) const
{
    return m_resource == rhs;
}

template<typename T>
bool ResourcePointer<T>::operator==(ResourcePointer<T> rhs) const
{
    return m_resource == rhs.m_resource;
}

template<typename T>
ResourcePointer<T>::operator bool() const
{
    return m_resource != nullptr;
}

template<typename T>
ResourcePointer<T>::~ResourcePointer()
{
    if(!m_counter)
    {
        return;
    }
    (*m_counter)--;

    if(m_counter->needRemove())
    {
        auto master = m_masterManager->value;
        if(master)
        {
            unsetMaster();
            master->remove(*this);
        }
        else
        {
            delete m_resource;
            delete m_counter;
            delete m_masterManager;
        }
    }
}

template<typename T>
ResourcePointer<T>::ResourcePointer( const ResourcePointer<T>& copy )
{
//    bool isSame = m_resource == copy.m_resource;
    m_resource = copy.m_resource;
    m_masterManager = copy.m_masterManager;
    m_counter = copy.m_counter;
    m_path = copy.m_path;
    if(m_counter)
    {
        (*m_counter)++;
    }
}

template<typename T>
ResourcePointer<T>::ResourcePointer( ResourcePointer<T>&& copy )
    : ResourcePointer<T>()
{
    swap(*this, copy);
}

template<typename T>
ResourcePointer<T>& ResourcePointer<T>::operator= ( ResourcePointer<T> copy )
{
    swap(*this, copy);
    return *this;
}
