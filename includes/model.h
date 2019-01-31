#pragma once

#include <vector>

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
    std::vector<Vertex> vertices;
    std::vector<Indice> indices;
    static Model readPSK( const QString& filename );
    std::vector<QString> materials;
    unsigned VAOsize;
};
