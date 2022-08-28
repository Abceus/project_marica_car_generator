#pragma once

#include "resources/model.h"
#include <vector>

#include "utils/gl.h"

struct Edge {
    GLuint vertexes[2];
};

struct WireframeModel {
    std::vector<Vertex> vertices;
    std::vector<Edge> edges;

    static WireframeModel fromModel(const Model& model);
};
