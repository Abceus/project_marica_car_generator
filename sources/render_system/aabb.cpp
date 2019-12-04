#include "render_system/aabb.h"
#include <QtMath>

Quade::Quade(QVector2D leftUp, QVector2D rightDown, float z )
    : m_leftUp( leftUp )
    , m_rightBottom( rightDown )
    , m_z( z )
{

}

QVector3D Quade::getLeftUp() const
{
    return QVector3D( m_leftUp, m_z );
}

QVector3D Quade::getLeftDown() const
{
    return QVector3D( m_leftUp.x(), m_rightBottom.y(), m_z );
}

QVector3D Quade::getRightUp() const
{
    return QVector3D( m_rightBottom.x(), m_leftUp.y(), m_z );
}

QVector3D Quade::getRightDown() const
{
    return QVector3D( m_rightBottom, m_z );
}

AABBBox::AABBBox(QVector3D leftUpNear, QVector3D rightDownFurther)
    : m_leftUpNear( std::move( leftUpNear ) )
    , m_rightDownFurther( std::move( rightDownFurther ) )
{

}

bool AABBBox::crossWith(const AABBBox &other)
{
    return crossWith( other.getNearQuade().getLeftUp() ) || // left up near
            crossWith( other.getNearQuade().getLeftDown() ) || // left down near
            crossWith( other.getNearQuade().getRightUp() ) || // right up near
            crossWith( other.getNearQuade().getRightDown() ) || // right down near

            crossWith( other.getFurtherQuade().getLeftUp() ) || // left up further
            crossWith( other.getFurtherQuade().getLeftDown() ) || // left down further
            crossWith( other.getFurtherQuade().getRightUp() ) || // right up further
            crossWith( other.getFurtherQuade().getRightDown() ); // right down further
}

bool AABBBox::crossWith(const QVector3D &point)
{
    return point.y() <= m_leftUpNear.y() && point.y() >= m_rightDownFurther.y() &&
           point.x() >= m_leftUpNear.x() && point.x() <= m_rightDownFurther.x() &&
            point.z() <= m_leftUpNear.z() && point.z() >= m_rightDownFurther.z();
}

Quade AABBBox::getNearQuade() const
{
    return Quade( m_leftUpNear.toVector2D(), m_rightDownFurther.toVector2D(), m_leftUpNear.z() );
}

Quade AABBBox::getFurtherQuade() const
{
    return Quade( m_leftUpNear.toVector2D(), m_rightDownFurther.toVector2D(), m_rightDownFurther.z() );
}

AABBBox AABBBox::createFromFaces(QVector<Face> faces, QVector<Vertex> vertexes, unsigned splitHalfTimes)
{
    float minX = vertexes[0].X;
    float maxX = vertexes[0].X;
    float minY = vertexes[0].Y;
    float maxY = vertexes[0].Y;
    float minZ = vertexes[0].Z;
    float maxZ = vertexes[0].Z;

    for( const auto& vertex: vertexes )
    {
        minX = std::min( minX, vertex.X );
        maxX = std::max( maxX, vertex.X );
        minY = std::min( minY, vertex.Y );
        maxY = std::max( maxY, vertex.Y );
        minZ = std::min( minZ, vertex.Z );
        maxZ = std::max( maxZ, vertex.Z );
    }

    QVector<QVector<AABBBox>> boxes;

    boxes.resize( qPow( 2, splitHalfTimes * 3 ) );

    QVector<float> xs;
    QVector<float> ys;
    QVector<float> zs;

    unsigned row = qPow( 2, splitHalfTimes );
    xs.resize( row );
    ys.resize( row );
    zs.resize( row );

    auto averageX = ( maxX - minX ) / row;
//    for( )
    return AABBBox();
}

AABBBox::AABBBox(QVector<AABBBox> childrens)
    : m_childrens( std::move( childrens ) )
{

}
