#include "vector3d.h"


Vector3D::Vector3D()
    : m_x( 0.0f )
    , m_y( 0.0f )
    , m_z( 0.0f )
{

}

Vector3D::Vector3D(float x, float y, float z)
    : m_x( x )
    , m_y( y )
    , m_z( z )
{
}

Vector3D::Vector3D(const QVector3D &base)
    : m_x( -base.z() )
    , m_y( base.x() )
    , m_z( base.y() )
{
}

Vector3D::Vector3D(const btVector3 &base)
    : m_x( -base.z() )
    , m_y( base.x() )
    , m_z( base.y() )
{
}

float Vector3D::x() const
{
    return m_x;
}

float Vector3D::y() const
{
    return m_y;
}

float Vector3D::z() const
{
    return m_z;
}

void Vector3D::setX(float x)
{
    m_x = x;
}

void Vector3D::setY(float y)
{
    m_y = y;
}

void Vector3D::setZ(float z)
{
    m_z = z;
}

QVector3D Vector3D::getQtVector() const
{
    return QVector3D( m_y, m_z, -m_x );
}

btVector3 Vector3D::getBulletVector() const
{
    return btVector3( m_y, m_z, -m_x );
}

Vector3D operator + (Vector3D const &v1, Vector3D const &v2)
{
    Vector3D result;
    result.setX( v1.x() + v2.x() );
    result.setY( v1.y() + v2.y() );
    result.setZ( v1.z() + v2.z() );
    return result;
}

Vector3D operator * (Vector3D const &v1, Vector3D const &v2)
{
    Vector3D result;
    result.setX( v1.x() * v2.x() );
    result.setY( v1.y() * v2.y() );
    result.setZ( v1.z() * v2.z() );
    return result;
}

Vector3D operator + (Vector3D const &v1, QVector3D const &v2)
{
    Vector3D result;
    result.setX( v1.x() - v2.z() );
    result.setY( v1.y() + v2.x() );
    result.setZ( v1.z() + v2.y() );
    return result;
}


Scale3D::Scale3D()
    : m_x( 0.0f )
    , m_y( 0.0f )
    , m_z( 0.0f )
{

}

Scale3D::Scale3D(float x, float y, float z)
    : m_x( x < 0 ? -x : x )
    , m_y( y < 0 ? -y : y )
    , m_z( z < 0 ? -z : z )
{
}

Scale3D::Scale3D(const QVector3D &base)
    : m_x( base.z() < 0 ? -base.z() : base.z() )
    , m_y( base.x() < 0 ? -base.x() : base.x() )
    , m_z( base.y() < 0 ? -base.y() : base.y() )
{
}

Scale3D::Scale3D(const btVector3 &base)
    : m_x( base.z() < 0 ? -base.z() : base.z() )
    , m_y( base.x() < 0 ? -base.x() : base.x() )
    , m_z( base.y() < 0 ? -base.y() : base.y() )
{
}

float Scale3D::x() const
{
    return m_x;
}

float Scale3D::y() const
{
    return m_y;
}

float Scale3D::z() const
{
    return m_z;
}

void Scale3D::setX(float x)
{
    m_x = x < 0 ? -x : x;
}

void Scale3D::setY(float y)
{
    m_y = y < 0 ? -y : y;
}

void Scale3D::setZ(float z)
{
    m_z = z < 0 ? -z : z;
}

QVector3D Scale3D::getQtVector() const
{
    return QVector3D( m_y, m_z, m_x );
}

btVector3 Scale3D::getBulletVector() const
{
    return btVector3( m_y, m_z, m_x );
}

Scale3D operator + (Scale3D const &v1, Scale3D const &v2)
{
    Scale3D result;
    result.setX( v1.x() + v2.x() );
    result.setY( v1.y() + v2.y() );
    result.setZ( v1.z() + v2.z() );
    return result;
}

Scale3D operator * (Scale3D const &v1, Scale3D const &v2)
{
    Scale3D result;
    result.setX( v1.x() * v2.x() );
    result.setY( v1.y() * v2.y() );
    result.setZ( v1.z() * v2.z() );
    return result;
}

Scale3D operator + (Scale3D const &v1, QVector3D const &v2)
{
    Scale3D result;
    result.setX( v1.x() + v2.z() );
    result.setY( v1.y() + v2.x() );
    result.setZ( v1.z() + v2.y() );
    return result;
}
