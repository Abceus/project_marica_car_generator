#include "collision_object.h"

#include "collision_parser.h"
#include "resources/wireframe_model.h"
#include "vector3d.h"
#include <QFile>
#include <QVector3D>
#include <QMatrix4x4>
#include <QtMath>


std::optional<CollisionObject> CollisionObject::readASE( const QString& path )
{
    QFile file( path );
    if ( !file.open( QIODevice::ReadOnly ) )
        return std::nullopt;

    auto ase = CollisionParser::parse( file.readAll() );
    file.close();

    QVector<CollisionObject> result;
    QVector<QMatrix4x4> transforms;

    for(const auto& gameObject: ase.gameObjects)
    {
        if(gameObject.mesh.vertices.empty()) continue;

        QMatrix4x4 transform;
        transform.setRow(0, {gameObject.tm_row0.x(), gameObject.tm_row0.y(), gameObject.tm_row0.z(), 0.0f});
        transform.setRow(1, {gameObject.tm_row1.x(), gameObject.tm_row1.y(), gameObject.tm_row1.z(), 0.0f});
        transform.setRow(2, {gameObject.tm_row2.x(), gameObject.tm_row2.y(), gameObject.tm_row2.z(), 0.0f});
        transform.setRow(3, {gameObject.tm_row3.x(), gameObject.tm_row3.y(), gameObject.tm_row3.z(), 0.0f});

        QVector<Vector3D> vertices;

        for(const auto& vert: gameObject.mesh.vertices)
        {
            vertices.append( vert.getQtVector() * transform.inverted() );
        }

        auto prefix = gameObject.name.left(6);
        if(prefix == "MCDBX_")
        {
            auto xMin = gameObject.mesh.vertices[0].x();
            auto xMax = gameObject.mesh.vertices[0].x();
            auto yMin = gameObject.mesh.vertices[0].y();
            auto yMax = gameObject.mesh.vertices[0].y();
            auto zMin = gameObject.mesh.vertices[0].z();
            auto zMax = gameObject.mesh.vertices[0].z();

            for(const auto& vertex: gameObject.mesh.vertices)
            {
                xMin = std::min(xMin, vertex.x());
                xMax = std::max(xMax, vertex.x());
                yMin = std::max(yMin, vertex.y());
                yMax = std::min(yMax, vertex.y());
                zMin = std::min(zMin, vertex.z());
                zMax = std::min(zMax, vertex.z());
            }

            auto xExtent = xMax - xMin;
            auto yExtent = yMax - yMin;
            auto zExtent = zMax - zMin;

            CollisionObject co;

            WireframeModel model;

            model.vertices.append({xMin, yMin, zMin});
            model.vertices.append({xMax, yMin, zMin});
            model.vertices.append({xMax, yMax, zMin});
            model.vertices.append({xMin, yMax, zMin});

            model.vertices.append({xMin, yMin, zMax});
            model.vertices.append({xMax, yMin, zMax});
            model.vertices.append({xMax, yMax, zMax});
            model.vertices.append({xMin, yMax, zMax});

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

            co.model = model;

#ifndef WITHOUT_SIMULATION
            co.shape = QSharedPointer<btBoxShape>( new btBoxShape( Vector3D(xExtent / 2.0f, yExtent / 2.0f, zExtent / 2.0f).getBulletVector() ) );
#endif
            result.push_back(co);
            transforms.push_back(transform);
        }
        else if(prefix == "MCDSP_")
        {
            QVector3D center{ transform.row(3).x(), transform.row(3).y(), transform.row(3).z() };
            auto radius = 0.0f;

            for(const auto& vert: gameObject.mesh.vertices)
            {
                radius += (vert.getQtVector() - center).length();
            }
            radius /= static_cast<float>(gameObject.mesh.vertices.size());

            WireframeModel model;

            for(size_t i = 0; i < 8; i++)
            {
                model.vertices.append({center.x() + static_cast<float>(qSin(i*45.0)*radius), center.y() + static_cast<float>(qCos(i*45.0)*radius), center.z()});

                model.edges.append({i, (i+1)%8});
            }

            for(size_t i = 0; i < 8; i++)
            {
                if(i == 0 || i == 4) continue;

                model.vertices.append({center.x() - static_cast<float>(qCos(i*45.0)*radius), center.y(), center.z() + static_cast<float>(qSin(i*45.0)*radius)});
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

                model.vertices.append({center.x(), center.y() + static_cast<float>(qCos(i*45.0)*radius), center.z() + static_cast<float>(qSin(i*45.0)*radius)});
            }

            model.edges.append({0, 14});
            model.edges.append({14, 9});
            model.edges.append({9, 15});
            model.edges.append({15, 4});
            model.edges.append({4, 16});
            model.edges.append({16, 12});
            model.edges.append({12, 17});
            model.edges.append({17, 0});

            result.append({model, QSharedPointer<btSphereShape>( new btSphereShape(radius) ) } );
            transforms.append(transform);
        }
        else if(prefix == "MCDCY_")
        {
            QVector3D center{ transform.row(3).x(), transform.row(3).y(), transform.row(3).z() };
            if(gameObject.mesh.vertices.empty()) continue;

            auto zMax = gameObject.mesh.vertices[0].z();
            auto zMin = gameObject.mesh.vertices[0].z();
            auto radius = 0.0f;

            for(const auto& vert: gameObject.mesh.vertices)
            {
                zMax = std::max(zMax, vert.z());
                zMin = std::min(zMax, vert.z());
                radius += (vert - Vector3D(center.x(), center.y(), vert.z())).getQtVector().length();
            }
            radius /= static_cast<float>(gameObject.mesh.vertices.size());

            WireframeModel model;

            for(size_t i = 0; i < 8; i++)
            {
                model.vertices.append({center.x() + static_cast<float>( qSin(45.0*i) * radius ), center.y() + static_cast<float>( qCos(45.0*i) * radius ), zMax });
                model.edges.append({i, (i+1)%8});
            }

            for(size_t i = 0; i < 8; i++)
            {
                model.vertices.append({center.x() + static_cast<float>( qSin(45.0*i) * radius ), center.y() + static_cast<float>( qCos(45.0*i) * radius ), zMin });
                model.edges.append({i+8, (i+1)%8+8});
            }

            for(size_t i = 0; i < 4; i+=2)
            {
                model.edges.append({i, i+8});
            }

            auto shape = QSharedPointer<btCylinderShapeZ>(new btCylinderShapeZ(btVector3(radius/2.0f, radius/2.0f, (zMax-zMin)/2.0f)));

            result.append({model, shape});
            transforms.append(transform);
        }
        else if(prefix == "MCDCX_")
        {
            CollisionObject co;
            Model originModel;
#ifndef WITHOUT_SIMULATION
            QSharedPointer<btConvexHullShape> shape(new btConvexHullShape);
#endif
            for(const auto& vert: gameObject.mesh.vertices)
            {
                originModel.vertices.push_back({vert.x(), vert.y(), vert.z(), 0.0f, 0.0f, 1});
            }

            for(const auto& face: gameObject.mesh.faces)
            {
                originModel.triangles.push_back({face.A, face.B, face.C});
#ifndef WITHOUT_SIMULATION
                shape->addPoint(vertices[face.A].getBulletVector());
                shape->addPoint(vertices[face.B].getBulletVector());
                shape->addPoint(vertices[face.C].getBulletVector());
#endif
            }
            auto wireframe = WireframeModel::fromModel(originModel);
            if(!wireframe)
            {
                continue;
            }
            co.model = wireframe.value();

#ifndef WITHOUT_SIMULATION
            co.shape = shape;
#endif
            result.push_back(co);
            transforms.push_back(transform);
        }
    }

    if(result.empty())
    {
        return std::nullopt;
    }

    if(result.size() == 1)
    {
        return result[0];
    }

    CollisionObject mergeResults;
    mergeResults.shape = QSharedPointer<btCompoundShape>(new btCompoundShape);

    for(size_t n = 0; n < result.size(); n++)
    {
        auto collisionObject = result[n];
        btMatrix3x3 rotation;
        for(size_t i = 0; i < 3; i++)
        {
            rotation[i][0] = transforms[n].row(i).x();
            rotation[i][1] = transforms[n].row(i).y();
            rotation[i][2] = transforms[n].row(i).z();
        }
        btVector3 position = { transforms[n].row(3).x(), transforms[n].row(3).y(), transforms[n].row(3).z() };
        mergeResults.shape.staticCast<btCompoundShape>()->addChildShape(btTransform(rotation, position), collisionObject.shape.get());
        // TODO: merge models
    }
    return mergeResults;
}
