#pragma once

#include "vector3d.h"
#include <QString>
#include <QVector>


struct ASEFile
{
    struct Face
    {
        unsigned int A, B, C;
    };


    struct Mesh
    {
        QVector<Vector3D> vertices;
        QVector<Face> faces;

    };


    struct GameObject
    {
        QString name;
        Vector3D tm_row0;
        Vector3D tm_row1;
        Vector3D tm_row2;
        Vector3D tm_row3;
        Vector3D tm_rotaxis;
        float tm_rotangle;
        Vector3D tm_pos;
        Mesh mesh;
    };

    QVector<GameObject> gameObjects;
};


class CollisionParser
{
public:
    static ASEFile parse(const QString& text);
private:
    template<typename Type>
    struct ValueOffset
    {
        Type value;
        int offset;
    };

    static ValueOffset<QString> parseString(const QString& text, const QString& token="", int offset=0);
    static ValueOffset<int> parseInt(const QString& text, const QString& token="", int offset=0);
    static ValueOffset<QVector<QString>> parseArray(const QString& text, const QString& elemenToken, const QString& token="", int offset=0);
    static ValueOffset<Vector3D> parseVertex(const QString& text, const QString& token="", int offset=0);
    static ValueOffset<ASEFile::Face> parseFace(const QString& text, const QString& token="", int offset=0);
};
