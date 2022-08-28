#include "resources/wireframe_model.h"

#include <algorithm>

WireframeModel WireframeModel::fromModel(const Model& model) {
    WireframeModel result;
    result.vertices =
        std::vector<Vertex>(model.vertices.begin(), model.vertices.end());

    std::sort(result.vertices.begin(), result.vertices.end(),
              [](const Vertex& a, const Vertex& b) {
                  if (a.X != b.X) {
                      return a.X < b.X;
                  }
                  if (a.Y != b.Y) {
                      return a.Y < b.Y;
                  }
                  return a.Z < b.Z;
              });

    auto vertexRemoveIterator =
        std::unique(result.vertices.begin(), result.vertices.end(),
                    [](const Vertex& a, const Vertex& b) {
                        return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
                    });
    result.vertices.erase(vertexRemoveIterator, result.vertices.end());

    std::vector<GLuint> vertexConformity;
    vertexConformity.reserve(model.vertices.size());

    for (const auto& vertex : model.vertices) {
        vertexConformity.push_back(static_cast<GLuint>(
            std::find_if(result.vertices.begin(), result.vertices.end(),
                         [vertex](const Vertex& a) {
                             return a.X == vertex.X && a.Y == vertex.Y &&
                                    a.Z == vertex.Z;
                         }) -
            result.vertices.begin()));
    }

    for (const auto& indice : model.faces) {
        for (size_t i = 0; i < 3; i++) {
            auto firstPoint = vertexConformity[indice.vertexes[i]];
            auto secondPoint = vertexConformity[indice.vertexes[(i + 1) % 3]];
            auto min = std::min(firstPoint, secondPoint);
            auto max = std::max(firstPoint, secondPoint);
            result.edges.push_back({min, max});
        }
    }

    std::sort(result.edges.begin(), result.edges.end(),
              [](const Edge& a, const Edge& b) {
                  return a.vertexes[0] == b.vertexes[0]
                             ? a.vertexes[1] < b.vertexes[1]
                             : a.vertexes[0] < b.vertexes[0];
              });
    auto edgeRemoveIterator =
        std::unique(result.edges.begin(), result.edges.end(),
                    [](const Edge& a, const Edge& b) {
                        return a.vertexes[0] == b.vertexes[0] &&
                               a.vertexes[1] == b.vertexes[1];
                    });
    result.edges.erase(edgeRemoveIterator, result.edges.end());
    return result;
}
