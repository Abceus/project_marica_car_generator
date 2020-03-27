#pragma once

class IUpdatable
{
public:
    virtual void update( float dt ) = 0;
    virtual ~IUpdatable() = default;
};
