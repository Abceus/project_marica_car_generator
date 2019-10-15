#include <QDebug>

#include "resources/unanimation.h"

ostream& operator<<( ostream& stream, VJointPos o )
{
    stream << "Orientation: " << o.Orientation << endl << "Postition: " << o.Position;
    return stream;
}

VJointPos VJointPos::operator+( VJointPos op2 )
{
    VJointPos temp;
    temp.Orientation = op2.Orientation + Orientation;
    temp.Position = op2.Position + Position;
    return temp;
}

VJointPos VJointPos::operator-( VJointPos op2 )
{
    VJointPos temp;
    temp.Orientation = op2.Orientation - Orientation;
    temp.Position = op2.Position - Position;
    return temp;
}

VJointPos VJointPos::operator+=( VJointPos op2 )
{
    Orientation = op2.Orientation + Orientation;
    Position = op2.Position + Position;
    return *this;
}

VJointPos& VJointPos::operator=( VJointPos op2 )
{
    Orientation = op2.Orientation;
    Position = op2.Position;
    return *this;
}

ostream& operator<<( ostream& stream, VQuat o )
{
    stream << "X: " << o.X << " Y: " << o.Y << " Z: " << o.Z << " W: " << o.W;
    return stream;
}

VQuat::VQuat( float vX, float vY, float vZ, float vW )
    : X( vX )
    , Y( vY )
    , Z( vZ )
    , W( vW )
{
}

VQuat VQuat::operator+( VQuat op2 )
{
    VQuat temp;
    temp.X = op2.X + X;
    temp.Y = op2.Y + Y;
    temp.Z = op2.Z + Z;
    temp.W = op2.W + W;
    return temp;
}

VQuat VQuat::operator-( VQuat op2 )
{
    VQuat temp;
    temp.X = op2.X - X;
    temp.Y = op2.Y - Y;
    temp.Z = op2.Z - Z;
    temp.W = op2.W - W;
    return temp;
}

VQuat VQuat::operator+= ( VQuat op2 )
{
    X = op2.X + X;
    Y = op2.Y + Y;
    Z = op2.Z + Z;
    W = op2.W + W;
    return *this;
}

VPoint::VPoint( float vX, float vY, float vZ)
        : X( vX )
        , Y( vY )
        , Z( vZ )
{
}

ostream& operator<<( ostream& stream, VPoint o )
{
    stream << "X: " << o.X << " Y: " << o.Y << " Z: " << o.Z;
    return stream;
}

VPoint VPoint::operator+( VPoint op2 )
{
    VPoint temp;
    temp.X = op2.X + X;
    temp.Y = op2.Y + Y;
    temp.Z = op2.Z + Z;
    return temp;
}

VPoint VPoint::operator-( VPoint op2 )
{
    VPoint temp;
    temp.X = op2.X - X;
    temp.Y = op2.Y - Y;
    temp.Z = op2.Z - Z;
    return temp;
}

VPoint VPoint::operator+=( VPoint op2 )
{
    X = op2.X + X;
    Y = op2.Y + Y;
    Z = op2.Z + Z;
    return *this;
}
