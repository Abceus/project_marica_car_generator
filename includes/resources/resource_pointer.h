#pragma once

template<typename T>
class IResourceMaster
{
public:
    virtual void remove( T* resource ) = 0;
};

class Counter
{
public:
    Counter();
    Counter operator++(int);
    Counter operator--(int);
    bool needRemove() const;
private:
    bool m_createFinished;
    size_t m_count;
};

template<typename T>
class ResourcePointer
{
public:
    ResourcePointer( IResourceMaster<T>* masterManager, T* resource );
    ~ResourcePointer();
    T* operator->();
    T *get();
    bool operator== (ResourcePointer<T> rhs) const;
    bool operator== (T* rhs) const;
    friend bool operator== (T* lhs, ResourcePointer<T> rhs);
private:
    T* m_resource;
    IResourceMaster<T> *m_masterManager;
    Counter* m_counter;
};

template<typename T>
bool operator== (T* lhs, ResourcePointer<T> rhs)
{
    return lhs == rhs.m_resource;
}

template<typename T>
ResourcePointer<T>::ResourcePointer(IResourceMaster<T> *masterManager, T *resource)
    : m_resource( resource )
    , m_masterManager( masterManager )
    , m_counter( new Counter )
{

}

template<typename T>
T *ResourcePointer<T>::operator->()
{
    return m_resource;
}

template<typename T>
T *ResourcePointer<T>::get()
{
    return m_resource;
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
ResourcePointer<T>::~ResourcePointer()
{
    if(!m_counter)
    {
        delete m_resource;
    } else
    {
        (*m_counter)--;
        if(m_counter->needRemove())
        {
            delete m_counter;
            m_counter = nullptr;
            m_masterManager->remove(m_resource);
        }
    }
}
