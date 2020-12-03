#pragma once

#include <vector>
#include <optional>

#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif
#include <GL/glu.h>
#include <GL/gl.h>

#include <QString>


struct Model
{
    struct Vertex
    {
        GLfloat X, Y, Z;
        GLfloat U, V;
        GLint MaterialIndex;
    };

    struct Triangle
    {
        GLuint vertexes[3];
    };

    Model() = default;
    Model( const Model& copy );
    std::vector<Vertex> vertices;
    std::vector<Triangle> triangles;
    static std::optional<Model> readPSK( const QString& filename );
    std::vector<QString> materials;
};
