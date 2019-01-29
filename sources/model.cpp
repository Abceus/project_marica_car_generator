#include "model.h"
#include <QFile>
#include <memory>
#include <includes/model.h>
#include <QOpenGLContext>
#include <QDebug>

#include "unanimation.h"

Model Model::readPSK( const QString &filename )
{
    auto newModel = Model();

    VChunkHeader GeneralHeader{};
    VChunkHeader PointsHeader{};
    std::vector<VPoint> PointsData;
    VChunkHeader WedgesHeader{};
    std::vector<VVertex> WedgesData;
    VChunkHeader FacesHeader{};
    std::vector<VTriangle> FacesData;
    VChunkHeader MaterialsHeader{};
    std::vector<VMaterial> MaterialsData;

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return newModel;
    QOpenGLFunctions* f = QOpenGLContext::currentContext()->functions();

    file.read(reinterpret_cast<char*>(&GeneralHeader), sizeof(VChunkHeader));
    qDebug() << GeneralHeader.ChunkID << " " << GeneralHeader.DataCount << " " << GeneralHeader.DataSize << " " << GeneralHeader.TypeFlags;
    file.read(reinterpret_cast<char*>(&PointsHeader), sizeof(VChunkHeader));
    qDebug() << PointsHeader.ChunkID << " " << PointsHeader.DataCount << " " << PointsHeader.DataSize << " " << PointsHeader.TypeFlags;
    PointsData.reserve(static_cast<size_t>(PointsHeader.DataCount));
    file.read( reinterpret_cast<char*>(PointsData.data()), PointsHeader.DataSize * PointsHeader.DataCount );

    file.read(reinterpret_cast<char*>(&WedgesHeader), sizeof(VChunkHeader));
    qDebug() << WedgesHeader.ChunkID << " " << WedgesHeader.DataCount << " " << WedgesHeader.DataSize << " " << WedgesHeader.TypeFlags;
    WedgesData.reserve(static_cast<size_t>(WedgesHeader.DataCount));
    file.read(reinterpret_cast<char*>(WedgesData.data()), WedgesHeader.DataSize * WedgesHeader.DataCount);

    file.read(reinterpret_cast<char*>(&FacesHeader), sizeof(VChunkHeader));
    qDebug() << FacesHeader.ChunkID << " " << FacesHeader.DataCount << " " << FacesHeader.DataSize << " " << FacesHeader.TypeFlags;
    FacesData.reserve(static_cast<size_t>(FacesHeader.DataCount));
    file.read(reinterpret_cast<char*>(FacesData.data()), FacesHeader.DataSize * FacesHeader.DataCount);

    file.read(reinterpret_cast<char*>(&MaterialsHeader), sizeof(VChunkHeader));
    qDebug() << MaterialsHeader.ChunkID << " " << MaterialsHeader.DataCount << " " << MaterialsHeader.DataSize << " " << MaterialsHeader.TypeFlags;
    MaterialsData.reserve(static_cast<size_t>(MaterialsHeader.DataCount));
    file.read(reinterpret_cast<char*>(MaterialsData.data()), MaterialsHeader.DataSize * MaterialsHeader.DataCount);

    file.close();

    newModel.vertices.resize(static_cast<size_t>(WedgesHeader.DataCount));

    for(size_t i=0; i<WedgesHeader.DataCount; i++)
    {
        newModel.vertices[i].X = PointsData[WedgesData[i].PointIndex].X;
        newModel.vertices[i].Y = PointsData[WedgesData[i].PointIndex].Y;
        newModel.vertices[i].Z = PointsData[WedgesData[i].PointIndex].Z;
        newModel.vertices[i].U = WedgesData[i].U;
        newModel.vertices[i].V = WedgesData[i].V;
        newModel.vertices[i].MaterialIndex = static_cast<GLint>(WedgesData[i].MatIndex);
    }

    newModel.indices.resize(static_cast<size_t>(FacesHeader.DataCount));

    for(size_t i=0; i<FacesHeader.DataCount; i++)
    {
        for(size_t j=0; j<3u; j++)
        {
            newModel.indices[i].vertexes[j] = FacesData[i].WedgeIndex[j];
        }
    }

    for(size_t i=0, i_max=static_cast<unsigned >(MaterialsHeader.DataCount); i<i_max; i++)
    {
        newModel.materials.emplace_back(QString("./") + QString(MaterialsData[i].MaterialName));
    }
    newModel.VAOsize = FacesHeader.DataCount*3u;
    return newModel;
}
