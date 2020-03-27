#pragma once

#include <QString>

template<typename T>
class ILoader
{
public:
    virtual T* load( const QString& path ) = 0;
    virtual void remove( T* path ) = 0;
};
