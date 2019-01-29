#ifndef MODEL_H
#define MODEL_H

#include <vector>

#include <QString>

#include <GL/glu.h>
#include <GL/gl.h>

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

#endif // MODEL_H
