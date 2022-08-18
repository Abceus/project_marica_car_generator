#pragma once

#include <string>
#include <vector>

#include "utils/gl.h"

struct Vertex {
    GLfloat X, Y, Z;
    GLfloat U, V;
    GLint MaterialIndex;
};

struct Face {
    GLuint vertexes[3];
};

struct Model {
    std::vector<Vertex> vertices;
    std::vector<Face> faces;
    std::vector<std::string> materials;

    static Model readPSK(const std::string& filename);
};
