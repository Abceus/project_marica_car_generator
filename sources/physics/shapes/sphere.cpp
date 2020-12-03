#include "physics/shapes/sphere.h"
#include "render_system/wireframe.h"

#include <QtMath>

Sphere::Sphere( float radius )
    : m_radius( radius )
{
}

QSharedPointer<Drawable> Sphere::makeDrawable() const
{
    WireframeModel model;

    for(size_t i = 0; i < 8; i++)
    {
        model.vertices.append({static_cast<float>(qSin(i*45.0)*m_radius), static_cast<float>(qCos(i*45.0)*m_radius), 0.0f});

        model.edges.append({i, (i+1)%8});
    }

    for(size_t i = 0; i < 8; i++)
    {
        if(i == 0 || i == 4) continue;

        model.vertices.append({-static_cast<float>(qCos(i*45.0)*m_radius), 0.0f, static_cast<float>(qSin(i*45.0)*m_radius)});
    }

    model.edges.append({6, 8});
    model.edges.append({8, 9});
    model.edges.append({9, 10});
    model.edges.append({10, 2});
    model.edges.append({2, 11});
    model.edges.append({11, 12});
    model.edges.append({12, 13});
    model.edges.append({13, 6});

    for(size_t i = 0; i < 8; i++)
    {
        if(i % 2 == 0) continue;

        model.vertices.append({0.0f,  static_cast<float>(qCos(i*45.0)*m_radius), static_cast<float>(qSin(i*45.0)*m_radius)});
    }

    model.edges.append({0, 14});
    model.edges.append({14, 9});
    model.edges.append({9, 15});
    model.edges.append({15, 4});
    model.edges.append({4, 16});
    model.edges.append({16, 12});
    model.edges.append({12, 17});
    model.edges.append({17, 0});

    return QSharedPointer<WireframeMesh>( new WireframeMesh( model ) );
}

#ifndef WITHOUT_SIMULATION
QSharedPointer<btCollisionShape> Sphere::makeBulletShape() const
{
    return QSharedPointer<btSphereShape>( new btSphereShape(m_radius) );
}
#endif
