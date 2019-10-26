#pragma once

#include <QVector3D>
#include <QVector2D>
#include <QVector>
#include <QSharedPointer>
#include "render_system/face.h"
#include "resources/model.h"

class Quade
{
public:
    Quade( QVector2D leftUp, QVector2D rightDown, float z );
    QVector3D getLeftUp() const;
    QVector3D getLeftDown() const;
    QVector3D getRightUp() const;
    QVector3D getRightDown() const;
private:
    QVector2D m_leftUp;
    QVector2D m_rightBottom;
    float m_z;
};

class AABBBox
{
public:
    AABBBox( QVector3D leftUpNear = QVector3D( 0.0f, 0.0f, 0.0f ), QVector3D rightDownFurther = QVector3D( 0.0f, 0.0f, 0.0f ) );
    bool crossWith( const AABBBox& other );
    bool crossWith( const QVector3D& point );
    Quade getNearQuade() const;
    Quade getFurtherQuade() const;

    static AABBBox createFromFaces( QVector<Face> faces, QVector<Vertex> vertexes, unsigned splitHalfTimes = 2 );
private:
    AABBBox( QVector<AABBBox> childrens );
    QVector3D m_leftUpNear;
    QVector3D m_rightDownFurther;
    QVector<QSharedPointer<AABBBox>> m_childrens;
    QVector<Face> m_innerFaces;
};
