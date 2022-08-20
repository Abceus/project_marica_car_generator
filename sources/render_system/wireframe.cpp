#include "render_system/wireframe.h"
#include "render_system/edge_array.h"
#include "render_system/element_buffer.h"
#include "render_system/shader_program.h"
#include "render_system/vertex_buffer.h"

#include <algorithm>
#include <memory>

void WireframeMesh::init(const Model& model, const Color& color) {
    m_color = color;

    auto vertices =
        std::vector<Vertex>(model.vertices.begin(), model.vertices.end());

    std::sort(vertices.begin(), vertices.end(),
              [](const Vertex& a, const Vertex& b) {
                  if (a.X != b.X) {
                      return a.X < b.X;
                  }
                  if (a.Y != b.Y) {
                      return a.Y < b.Y;
                  }
                  return a.Z < b.Z;
              });

    auto vertexRemoveIterator = std::unique(
        vertices.begin(), vertices.end(), [](const Vertex& a, const Vertex& b) {
            return a.X == b.X && a.Y == b.Y && a.Z == b.Z;
        });
    vertices.erase(vertexRemoveIterator, vertices.end());

    std::vector<Edge> edges;

    std::vector<GLuint> vertexConformity;
    vertexConformity.reserve(model.vertices.size());

    for (const auto& vertex : model.vertices) {
        vertexConformity.push_back(static_cast<GLuint>(
            std::find_if(vertices.begin(), vertices.end(),
                         [vertex](const Vertex& a) {
                             return a.X == vertex.X && a.Y == vertex.Y &&
                                    a.Z == vertex.Z;
                         }) -
            vertices.begin()));
    }

    for (const auto& indice : model.faces) {
        for (size_t i = 0; i < 3; i++) {
            auto firstPoint = vertexConformity[indice.vertexes[i]];
            auto secondPoint = vertexConformity[indice.vertexes[(i + 1) % 3]];
            auto min = std::min(firstPoint, secondPoint);
            auto max = std::max(firstPoint, secondPoint);
            edges.push_back({min, max});
        }
    }

    std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
        return a.vertexes[0] == b.vertexes[0] ? a.vertexes[1] < b.vertexes[1]
                                              : a.vertexes[0] < b.vertexes[0];
    });
    auto edgeRemoveIterator = std::unique(
        edges.begin(), edges.end(), [](const Edge& a, const Edge& b) {
            return a.vertexes[0] == b.vertexes[0] &&
                   a.vertexes[1] == b.vertexes[1];
        });
    edges.erase(edgeRemoveIterator, edges.end());

    auto vbo = std::make_shared<VertexBuffer>();
    vbo->init(vertices);

    auto ebo = std::make_shared<ElementBuffer>();
    ebo->init(edges);

    edgeArray = std::make_shared<EdgeArray>();
    edgeArray->init(vbo, ebo);
}

void WireframeMesh::draw(ShaderProgram* shaderProgram) const {
    shaderProgram->setUniform("color", m_color.getGLVec4());
    edgeArray->draw();
}
