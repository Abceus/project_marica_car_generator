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

struct Vertex
{
    GLfloat X, Y, Z;
    GLfloat U, V;
    GLint MaterialIndex;
};

struct Indice
{
    GLuint vertexes[3];
};

struct Model
{
    Model() = default;
    Model( const Model& copy );
    std::vector<Vertex> vertices;
    std::vector<Indice> indices;
    static std::optional<Model> readPSK( const QString& filename );
    std::vector<QString> materials;
    unsigned VAOsize = 0;
};
