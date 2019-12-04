#pragma once

#include <QVector3D>

#ifndef WITHOUT_SIMULATION
#include "btBulletCollisionCommon.h"
#endif

class Vector3D
{
public:
    Vector3D();
    Vector3D( float x, float y, float z );
    Vector3D( const QVector3D& base );
#ifndef WITHOUT_SIMULATION
    Vector3D( const btVector3& base );
#endif

//    Vector3D &operator =( const QVector3D& other );
//    Vector3D &operator =( QVector3D&& other );
//    Vector3D &operator =( const btVector3& other );

    float x() const;
    float y() const;
    float z() const;

    void setX( float x );
    void setY( float y );
    void setZ( float z );

    QVector3D getQtVector() const;
#ifndef WITHOUT_SIMULATION
    btVector3 getBulletVector() const;
#endif

    friend Vector3D operator +(Vector3D const &v1, Vector3D const &v2);
    friend Vector3D operator *(Vector3D const &v1, Vector3D const &v2);

    friend Vector3D operator +(Vector3D const &v1, QVector3D const &v2);
private:
    float m_x;
    float m_y;
    float m_z;
};


class Scale3D
{
public:
    Scale3D();
    Scale3D( float x, float y, float z );
    Scale3D( const QVector3D& base );
#ifndef WITHOUT_SIMULATION
    Scale3D( const btVector3& base );
#endif

    float x() const;
    float y() const;
    float z() const;

    void setX( float x );
    void setY( float y );
    void setZ( float z );

    QVector3D getQtVector() const;
#ifndef WITHOUT_SIMULATION
    btVector3 getBulletVector() const;
#endif

    friend Scale3D operator +(Scale3D const &v1, Scale3D const &v2);
    friend Scale3D operator *(Scale3D const &v1, Scale3D const &v2);

    friend Scale3D operator +(Scale3D const &v1, QVector3D const &v2);
private:
    float m_x;
    float m_y;
    float m_z;
};
