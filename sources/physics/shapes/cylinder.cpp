#include "physics/shapes/cylinder.h"
#include "render_system/wireframe.h"

#include <QtMath>


Cylinder::Cylinder( float radius, float height )
    : m_radius( radius )
    , m_height( height )
{}

QSharedPointer<Drawable> Cylinder::makeDrawable() const
{
    WireframeModel model;

    for(size_t i = 0; i < 8; i++)
    {
        model.vertices.append({static_cast<float>( qSin(45.0*i) * m_radius ), static_cast<float>( qCos(45.0*i) * m_radius ), m_height });
        model.edges.append({i, (i+1)%8});
    }

    for(size_t i = 0; i < 8; i++)
    {
        model.vertices.append({static_cast<float>( qSin(45.0*i) * m_radius ), static_cast<float>( qCos(45.0*i) * m_radius ), m_height });
        model.edges.append({i+8, (i+1)%8+8});
    }

    for(size_t i = 0; i < 4; i+=2)
    {
        model.edges.append({i, i+8});
    }

    return QSharedPointer<WireframeMesh>( new WireframeMesh( model ) );
}

#ifndef WITHOUT_SIMULATION
QSharedPointer<btCollisionShape> Cylinder::makeBulletShape() const
{
    return QSharedPointer<btCylinderShapeZ>(new btCylinderShapeZ(btVector3(m_radius, m_radius, m_height/2.0f)));
}
#endif
