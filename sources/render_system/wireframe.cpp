#include "render_system/wireframe.h"
#include "render_system/edge_array.h"
#include "render_system/element_buffer.h"
#include "render_system/shader_program.h"
#include "render_system/vertex_buffer.h"

#include <algorithm>
#include <memory>

void WireframeMesh::init(const WireframeModel& model, const Color& color) {
    m_color = color;

    auto vbo = std::make_shared<VertexBuffer>();
    vbo->init(model.vertices);

    auto ebo = std::make_shared<ElementBuffer>();
    ebo->init(model.edges);

    edgeArray = std::make_shared<EdgeArray>();
    edgeArray->init(vbo, ebo);
}

void WireframeMesh::draw(ShaderProgram* shaderProgram) const {
    shaderProgram->setUniform("color", m_color.getGLVec4());
    edgeArray->draw();
}
