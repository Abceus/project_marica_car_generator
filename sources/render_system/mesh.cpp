#include "render_system/mesh.h"
#include <memory>

#include "render_system/element_buffer.h"
#include "render_system/triangle_array.h"
#include "render_system/vertex_buffer.h"

#include <wx/filefn.h>

void Mesh::init(const Model& model) {
    // batches.resize(model.materials.size());
    // for(auto& batch: batches) {
    //     batch = std::make_shared<Batch>();
    // }
    batches.clear();

    auto vertexBuffer = std::make_shared<VertexBuffer>();
    vertexBuffer->init(model.vertices);

    std::vector<std::vector<Face>> elementBuffers;
    elementBuffers.resize(model.materials.size());
    // for(auto& elementBuffer: elementBuffers) {
    //     elementBuffer = std::make_shared<ElementBuffer>();
    // }

    for (const auto& face : model.faces) {
        elementBuffers[model.vertices[face.vertexes[0]].MaterialIndex]
            .push_back(face);
    }

    // auto texture = std::make_shared<Texture>();
    // texture->init(
    //     wxImage("D:\\Documents\\gits\\project_marica_car_"
    //             "generator\\example\\MaricaFlatoutTex\\Texture\\common.png"));
    int i = 0;
    for (const auto& elementBuffer : elementBuffers) {
        auto vertexArray = std::make_shared<TriangleArray>();

        auto newElementBuffer = std::make_shared<ElementBuffer>();
        newElementBuffer->init(elementBuffer);

        vertexArray->init(vertexBuffer, newElementBuffer);

        auto texture = std::make_shared<Texture>();
        wxImage image;
        if (wxFileExists(model.materials[i])) {
            image = wxImage(model.materials[i]);
        } else {
            image = wxImage(
                "D:\\Documents\\gits\\project_marica_car_"
                "generator\\example\\MaricaFlatoutTex\\Texture\\common.png");
        }
        texture->init(image);

        batches.emplace_back(std::make_unique<Batch>(vertexArray, texture));
        ++i;
    }
}

void Mesh::setTexture(const std::shared_ptr<Texture>& texture, size_t index) {}

void Mesh::addTexture(const std::shared_ptr<Texture>& texture) {}

void Mesh::draw(ShaderProgram* shaderProgram) const {
    for (const auto& batch : batches) {
        batch->draw();
    }
}