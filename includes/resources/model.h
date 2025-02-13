#pragma once

#include <filesystem>
#include <map>
#include <memory>
#include <set>
#include <string>
#include <vcruntime.h>
#include <vector>


#include "resources/skeletal.h"
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

    Skeletal skeletal;
    std::map<size_t, std::set<std::string>> vlinks;
    std::map<std::string, std::vector<std::pair<size_t, float>>> blinks;

    static Model readPSK(const std::filesystem::path& filename);
    static std::vector<Model> readASE(const std::filesystem::path& filename);
};
