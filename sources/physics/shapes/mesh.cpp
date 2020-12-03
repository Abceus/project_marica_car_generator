#include "physics/shapes/mesh.h"
#include "render_system/wireframe.h"

Mesh::Mesh( ResourcePointer<Model> model )
    : m_model( model )
{
}

QSharedPointer<Drawable> Mesh::makeDrawable() const
{
    auto m = *m_model.get();
    return QSharedPointer<WireframeMesh>( new WireframeMesh( WireframeModel::fromModel(m).value() ) );
}

#ifndef WITHOUT_SIMULATION
QSharedPointer<btCollisionShape> Mesh::makeBulletShape() const
{
    QSharedPointer<btConvexHullShape> shape(new btConvexHullShape);

    for(const auto& triangle: m_model->triangles)
    {
        for(size_t i = 0; i < 3; i++)
        {
            const auto& vertex = m_model->vertices[triangle.vertexes[i]];
            shape->addPoint(Vector3D( vertex.X, vertex.Y, vertex.Z ).getBulletVector());
        }
    }
    return shape;
}
#endif
