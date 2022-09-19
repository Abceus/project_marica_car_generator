#include "render_system/mesh.h"
#include <memory>

#include "glm/fwd.hpp"
#include "render_system/element_buffer.h"
#include "render_system/triangle_array.h"
#include "render_system/vertex_buffer.h"
#include "resources/skeletal.h"
#include "utils/math/matrix.h"
#include "utils/math/quaternion.h"

#include <wx/filefn.h>

void Mesh::init(const Model& m) {
    model = m;
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
                "generator\\example\\MaricaFlatoutTex\\Texture\\common.tga");
        }
        texture->init(image);

        batches.emplace_back(std::make_unique<Batch>(vertexArray, texture));
        ++i;
    }

    for (const auto& vl : model.vlinks) {
        vmap.emplace(&model.vertices[vl.first], vl.first);
        vlinks.emplace(&model.vertices[vl.first], std::vector<Bone*>());
        auto foundVertex = vlinks.find(&model.vertices[vl.first]);

        for (const auto& bone : vl.second) {
            foundVertex->second.emplace_back(
                model.skeletal.getBone(bone).lock().get());
        }
    }

    for (const auto& bl : model.blinks) {
        blinks.emplace(model.skeletal.getBone(bl.first).lock().get(),
                       std::vector<std::pair<Vertex*, float>>());
        auto foundBone =
            blinks.find(model.skeletal.getBone(bl.first).lock().get());

        for (const auto& point : bl.second) {
            foundBone->second.push_back(
                {&model.vertices[point.first], point.second});
        }
    }
}

void Mesh::setTexture(const std::shared_ptr<Texture>& texture, size_t index) {
    batches[index]->setTexture(texture);
}

void Mesh::draw(ShaderProgram* shaderProgram) const {
    if (!updates.isEmpty()) {
        for (const auto& update : updates) {
            if (!batches.empty()) {
                batches[0]
                    ->getVertexArray()
                    .lock()
                    ->getVertexBuffer()
                    .lock()
                    ->updateBuffer(update.getFirst(), update.getData());
            }
        }
        updates.clear();
    }

    for (const auto& batch : batches) {
        batch->draw();
    }
}

void Mesh::rotateBone(const std::string& boneName,
                      const Quaternion& quaternion) {
    auto bone = model.skeletal.getBone(boneName).lock();
    bone->getTransform().setRotation(quaternion);

    auto blink = blinks.find(bone.get())->second;
    for (const auto& vertex : blink) {
        auto vlink = vlinks.find(vertex.first);
        Matrixf44 resultMatrix(1.0f);
        for (const auto& b : vlink->second) {
            resultMatrix = b->getGlobalOriginMatrix() * resultMatrix *
                           (b->getGlobalMatrix() * vertex.second) *
                           b->getGlobalOriginMatrix().getInverted();
            // resultMatrix = b->getGlobalOriginMatrix().getInverted();
        }
        Range<Vertex> update;
        update.setFirst(vmap.find(vertex.first)->second);
        Vertex resultVertex = *vertex.first;
        auto q =
            Matrixf44::apply(resultMatrix, Vec3f(resultVertex.X, resultVertex.Y,
                                                 resultVertex.Z));
        resultVertex.X = q.getX();
        resultVertex.Y = q.getY();
        resultVertex.Z = q.getZ();
        update.setData({resultVertex});
        updates.add(update);
    }
}

void Mesh::transposeBone(const std::string& boneName, const Vec3f& position) {
    auto bone = model.skeletal.getBone(boneName).lock();
    bone->getTransform().setLocation(position);

    auto blink = blinks.find(bone.get())->second;
    for (const auto& vertex : blink) {
        auto vlink = vlinks.find(vertex.first);
        Matrixf44 resultMatrix(1.0f);
        for (const auto& b : vlink->second) {
            resultMatrix = b->getGlobalOriginMatrix() * resultMatrix *
                           (b->getGlobalMatrix() * vertex.second) *
                           b->getGlobalOriginMatrix().getInverted();
        }
        Range<Vertex> update;
        update.setFirst(vmap.find(vertex.first)->second);
        Vertex resultVertex = *vertex.first;
        auto q =
            Matrixf44::apply(resultMatrix, Vec3f(resultVertex.X, resultVertex.Y,
                                                 resultVertex.Z));
        resultVertex.X = q.getX();
        resultVertex.Y = q.getY();
        resultVertex.Z = q.getZ();
        update.setData({resultVertex});
        updates.add(update);
    }
}