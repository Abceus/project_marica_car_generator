#include "physics/shapes/box.h"
#include "render_system/wireframe.h"

Box::Box( float xExtend, float yExtend, float zExtend )
    : m_xExtend(xExtend)
    , m_yExtend(yExtend)
    , m_zExtend(zExtend)
{
}

QSharedPointer<Drawable> Box::makeDrawable() const
{
    auto halfXExtend = m_xExtend / 2.0f;
    auto halfYExtend = m_yExtend / 2.0f;
    auto halfZExtend = m_zExtend / 2.0f;

    WireframeModel model;

    model.vertices.append({-halfXExtend, -halfYExtend, -halfZExtend});
    model.vertices.append({halfXExtend, -halfYExtend, -halfZExtend});
    model.vertices.append({halfXExtend, halfYExtend, -halfZExtend});
    model.vertices.append({-halfXExtend, halfYExtend, -halfZExtend});

    model.vertices.append({-halfXExtend, -halfYExtend, halfZExtend});
    model.vertices.append({halfXExtend, -halfYExtend, halfZExtend});
    model.vertices.append({halfXExtend, halfYExtend, halfZExtend});
    model.vertices.append({-halfXExtend, halfYExtend, halfZExtend});

    model.edges = { { 0, 1 },
                    { 1, 2 },
                    { 2, 3 },
                    { 3, 0 },
                    { 4, 5 },
                    { 5, 6 },
                    { 6, 7 },
                    { 7, 4 },
                    { 0, 4 },
                    { 1, 5 },
                    { 2, 6 },
                    { 3, 7 } };

    return QSharedPointer<WireframeMesh>( new WireframeMesh( model ) );
}

#ifndef WITHOUT_SIMULATION
QSharedPointer<btCollisionShape> Box::makeBulletShape() const
{
    auto halfXExtend = m_xExtend / 2.0f;
    auto halfYExtend = m_yExtend / 2.0f;
    auto halfZExtend = m_zExtend / 2.0f;

    return QSharedPointer<btBoxShape>( new btBoxShape( Vector3D(halfXExtend, halfYExtend, halfZExtend).getBulletVector() ) );

}
#endif
