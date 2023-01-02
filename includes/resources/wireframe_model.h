#pragma once

#include "resources/model.h"
#include <vector>

#include <mul_opengl.h>

struct Edge {
    GLuint vertexes[2];
};

struct WireframeModel {
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    static WireframeModel fromModel(const Model& model);
};
