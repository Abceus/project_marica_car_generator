#include <fstream>
#include <ios>
#include <memory>

#include "resources/model.h"
#include "resources/unanimation.h"

#include "resources/ase_reader/ase_reader.h"
#include "utils/math/quaternion.h"

Model Model::readPSK(const std::string& filename) {
    auto newModel = Model();

    VChunkHeader GeneralHeader;
    VChunkHeader PointsHeader;
    std::vector<VPoint> PointsData;
    VChunkHeader WedgesHeader;
    std::vector<VVertex> WedgesData;
    VChunkHeader FacesHeader;
    std::vector<VTriangle> FacesData;
    VChunkHeader MaterialsHeader;
    std::vector<VMaterial> MaterialsData;
    VChunkHeader BonesHeader;
    std::vector<VBone> BonesData;
    VChunkHeader InfluencesHeader;
    std::vector<VRawBoneInfluence> InfluencesData;

    std::ifstream file(filename, std::ios_base::in | std::ios_base::binary);
    if (!file.is_open())
        return newModel;

    file.read(reinterpret_cast<char*>(&GeneralHeader), sizeof(VChunkHeader));
    file.read(reinterpret_cast<char*>(&PointsHeader), sizeof(VChunkHeader));
    PointsData.resize(static_cast<size_t>(PointsHeader.DataCount));
    file.read(reinterpret_cast<char*>(PointsData.data()),
              PointsHeader.DataSize * PointsHeader.DataCount);

    file.read(reinterpret_cast<char*>(&WedgesHeader), sizeof(VChunkHeader));
    WedgesData.resize(static_cast<size_t>(WedgesHeader.DataCount));
    file.read(reinterpret_cast<char*>(WedgesData.data()),
              WedgesHeader.DataSize * WedgesHeader.DataCount);

    file.read(reinterpret_cast<char*>(&FacesHeader), sizeof(VChunkHeader));
    FacesData.resize(static_cast<size_t>(FacesHeader.DataCount));
    file.read(reinterpret_cast<char*>(FacesData.data()),
              FacesHeader.DataSize * FacesHeader.DataCount);

    file.read(reinterpret_cast<char*>(&MaterialsHeader), sizeof(VChunkHeader));
    MaterialsData.resize(static_cast<size_t>(MaterialsHeader.DataCount));
    file.read(reinterpret_cast<char*>(MaterialsData.data()),
              MaterialsHeader.DataSize * MaterialsHeader.DataCount);

    file.read(reinterpret_cast<char*>(&BonesHeader), sizeof(VChunkHeader));
    BonesData.resize(static_cast<size_t>(BonesHeader.DataCount));
    file.read(reinterpret_cast<char*>(BonesData.data()),
              BonesHeader.DataSize * BonesHeader.DataCount);

    file.read(reinterpret_cast<char*>(&InfluencesHeader), sizeof(VChunkHeader));
    InfluencesData.resize(static_cast<size_t>(InfluencesHeader.DataCount));
    file.read(reinterpret_cast<char*>(InfluencesData.data()),
              InfluencesHeader.DataSize * InfluencesHeader.DataCount);

    file.close();

    newModel.vertices.resize(static_cast<size_t>(WedgesHeader.DataCount));
    std::map<size_t, std::vector<size_t>> plinks;
    for (size_t i = 0; i < WedgesHeader.DataCount; i++) {
        newModel.vertices[i].X = PointsData[WedgesData[i].PointIndex].X;
        newModel.vertices[i].Y = PointsData[WedgesData[i].PointIndex].Y;
        newModel.vertices[i].Z = PointsData[WedgesData[i].PointIndex].Z;
        newModel.vertices[i].U = WedgesData[i].U;
        newModel.vertices[i].V = WedgesData[i].V;
        newModel.vertices[i].MaterialIndex = WedgesData[i].MatIndex;
        plinks[WedgesData[i].PointIndex].push_back(i);
    }

    newModel.faces.resize(static_cast<size_t>(FacesHeader.DataCount));

    for (size_t i = 0; i < FacesHeader.DataCount; i++) {
        for (size_t j = 0; j < 3u; j++) {
            newModel.faces[i].vertexes[j] = FacesData[i].WedgeIndex[j];
        }
    }

    for (size_t i = 0, i_max = static_cast<unsigned>(MaterialsHeader.DataCount);
         i < i_max; i++) {
        newModel.materials.emplace_back(
            std::string("./") + std::string(MaterialsData[i].MaterialName));
    }

    std::vector<std::string> boneNames;
    for (size_t i = 0; i < static_cast<unsigned>(BonesHeader.DataCount); ++i) {
        auto data = BonesData[i];
        auto newBone = std::make_shared<Bone>();
        // newBone->getOrigin().setLocation({data.BonePos.Position.X,
        //                                      data.BonePos.Position.Y,
        //                                      data.BonePos.Position.Z});
        newBone->getOrigin().setLocation({data.BonePos.Position.X,
                                             data.BonePos.Position.Y,
                                             data.BonePos.Position.Z});

        newBone->getOrigin().setRotation(
            Quaternion(data.BonePos.Orientation.X, data.BonePos.Orientation.Y,
                       data.BonePos.Orientation.Z, data.BonePos.Orientation.W));

        newModel.skeletal.addBone(data.Name, newBone);
        boneNames.push_back(data.Name);
    }

    for (size_t i = 0; i < static_cast<unsigned>(BonesHeader.DataCount); ++i) {
        auto data = BonesData[i];
        auto bone = newModel.skeletal.getBone(boneNames[i]).lock();
        if (i != 0) {
            auto parentBone =
                newModel.skeletal.getBone(boneNames[data.ParentIndex]).lock();
            parentBone->addChild(bone);
        }
    }

    for(size_t i = 0; i < static_cast<unsigned>(InfluencesHeader.DataCount); ++i) {
        auto data = InfluencesData[i];
        auto vertexIndexs = plinks[data.PointIndex];
        for(const auto& vertexIndex: vertexIndexs) {
            auto foundVertex = newModel.vlinks.find(vertexIndex);
            if(foundVertex == std::end(newModel.vlinks)) {
                newModel.vlinks.emplace(static_cast<size_t>(vertexIndex), std::set<std::string>());
                foundVertex = newModel.vlinks.find(vertexIndex);
            }
            foundVertex->second.insert(boneNames[data.BoneIndex]);
        }
        
        auto foundBone = newModel.blinks.find(boneNames[data.BoneIndex]);
        if(foundBone == std::end(newModel.blinks)) {
            newModel.blinks.emplace(boneNames[data.BoneIndex], std::vector<std::pair<size_t, float>>());
            foundBone = newModel.blinks.find(boneNames[data.BoneIndex]);
        }
        for(const auto& vertexIndex: vertexIndexs) {
            foundBone->second.push_back({vertexIndex, data.Weight});
        }
    }

    return newModel;
}

std::vector<Model> Model::readASE(const std::string& filename) {
    ASEReader reader;
    reader.init(filename);

    std::vector<Model> result;
    if (auto lockedRootNode = reader.getRootNode().lock()) {
        for (auto gameObjectNum = 0;
             gameObjectNum < lockedRootNode->getChild("GEOMOBJECT")->getSize();
             ++gameObjectNum) {
            result.push_back({});
            result.back().materials.emplace_back("");

            for (auto vertexNum = 0;
                 vertexNum < lockedRootNode->getChild("GEOMOBJECT")
                                 ->getElement(gameObjectNum)
                                 ->getChild("MESH")
                                 ->getChild("MESH_NUMVERTEX")
                                 ->getValue<int>();
                 ++vertexNum) {
                auto vertexNode = lockedRootNode->getChild("GEOMOBJECT")
                                      ->getElement(gameObjectNum)
                                      ->getChild("MESH")
                                      ->getChild("MESH_VERTEX_LIST")
                                      ->getElement(vertexNum)
                                      ->getChild("MESH_VERTEX");
                result.back().vertices.push_back(
                    {vertexNode->getElement(0)->getValue<float>(),
                     vertexNode->getElement(1)->getValue<float>(),
                     vertexNode->getElement(2)->getValue<float>(), 0.0f, 0.0f,
                     0});
            }

            for (auto faceNum = 0;
                 faceNum < lockedRootNode->getChild("GEOMOBJECT")
                               ->getElement(gameObjectNum)
                               ->getChild("MESH")
                               ->getChild("MESH_NUMFACES")
                               ->getValue<int>();
                 ++faceNum) {

                auto faceNode = lockedRootNode->getChild("GEOMOBJECT")
                                    ->getElement(gameObjectNum)
                                    ->getChild("MESH")
                                    ->getChild("MESH_FACE_LIST")
                                    ->getElement(faceNum)
                                    ->getChild("MESH_FACE");

                result.back().faces.push_back(
                    {{static_cast<GLuint>(faceNode->getElement(0)
                                              ->getElement(1)
                                              ->getValue<int>()),
                      static_cast<GLuint>(faceNode->getElement(1)
                                              ->getElement(1)
                                              ->getValue<int>()),
                      static_cast<GLuint>(faceNode->getElement(2)
                                              ->getElement(1)
                                              ->getValue<int>())}});
            }
        }
    }
    return result;
}