#pragma once

#include <chrono>

class IUpdatable
{
public:
    virtual void update( const std::chrono::milliseconds& dt ) = 0;
};
