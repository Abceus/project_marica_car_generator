#include <memory>

#include <QFile>
#include <QOpenGLContext>
#include <QDebug>

#include "resources/model.h"
#include "resources/unanimation.h"

Model::Model(const Model &copy)
{
    vertices = copy.vertices;
    indices = copy.indices;
    materials = copy.materials;
    VAOsize = copy.VAOsize;
}

std::optional<Model> Model::readPSK( const QString &filename )
{
    VChunkHeader GeneralHeader{};
    VChunkHeader PointsHeader{};
    std::vector<VPoint> PointsData;
    VChunkHeader WedgesHeader{};
    std::vector<VVertex> WedgesData;
    VChunkHeader FacesHeader{};
    std::vector<VTriangle> FacesData;
    VChunkHeader MaterialsHeader{};
    std::vector<VMaterial> MaterialsData;
    VChunkHeader BonesHeader{};
    std::vector<VBone> BonesData;

    QFile file( filename );
    if ( !file.open( QIODevice::ReadOnly ) )
        return std::nullopt;

    auto newModel = Model();

    file.read( reinterpret_cast<char*>( &GeneralHeader ), sizeof( VChunkHeader ) );
    qDebug() << GeneralHeader.ChunkID << " " << GeneralHeader.DataCount << " " << GeneralHeader.DataSize << " " << GeneralHeader.TypeFlags;
    file.read( reinterpret_cast<char*>( &PointsHeader ), sizeof( VChunkHeader ) );
    qDebug() << PointsHeader.ChunkID << " " << PointsHeader.DataCount << " " << PointsHeader.DataSize << " " << PointsHeader.TypeFlags;
    PointsData.resize( static_cast<size_t>( PointsHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( PointsData.data() ), PointsHeader.DataSize * PointsHeader.DataCount );

    file.read( reinterpret_cast<char*>( &WedgesHeader ), sizeof( VChunkHeader ) );
    qDebug() << WedgesHeader.ChunkID << " " << WedgesHeader.DataCount << " " << WedgesHeader.DataSize << " " << WedgesHeader.TypeFlags;
    WedgesData.resize( static_cast<size_t>( WedgesHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( WedgesData.data() ), WedgesHeader.DataSize * WedgesHeader.DataCount );

    file.read( reinterpret_cast<char*>( &FacesHeader ), sizeof( VChunkHeader ) );
    qDebug() << FacesHeader.ChunkID << " " << FacesHeader.DataCount << " " << FacesHeader.DataSize << " " << FacesHeader.TypeFlags;
    FacesData.resize( static_cast<size_t>( FacesHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( FacesData.data() ), FacesHeader.DataSize * FacesHeader.DataCount );

    file.read( reinterpret_cast<char*>( &MaterialsHeader ), sizeof( VChunkHeader ) );
    qDebug() << MaterialsHeader.ChunkID << " " << MaterialsHeader.DataCount << " " << MaterialsHeader.DataSize << " " << MaterialsHeader.TypeFlags;
    MaterialsData.resize( static_cast<size_t>( MaterialsHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( MaterialsData.data() ), MaterialsHeader.DataSize * MaterialsHeader.DataCount );

    file.read( reinterpret_cast<char*>( &BonesHeader ), sizeof( VChunkHeader ) );
    qDebug() << BonesHeader.ChunkID << " " << BonesHeader.DataCount << " " << BonesHeader.DataSize << " " << BonesHeader.TypeFlags;
    BonesData.resize( static_cast<size_t>( BonesHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( BonesData.data() ), BonesHeader.DataSize * BonesHeader.DataCount );

    file.close();

    newModel.vertices.resize( static_cast<size_t>( WedgesHeader.DataCount ) );

    for( size_t i=0; i<WedgesHeader.DataCount; i++ )
    {
        newModel.vertices[i].X = PointsData[WedgesData[i].PointIndex].X;
        newModel.vertices[i].Y = PointsData[WedgesData[i].PointIndex].Y;
        newModel.vertices[i].Z = PointsData[WedgesData[i].PointIndex].Z;
        newModel.vertices[i].U = WedgesData[i].U;
        newModel.vertices[i].V = WedgesData[i].V;
        newModel.vertices[i].MaterialIndex = WedgesData[i].MatIndex;
    }

    newModel.indices.resize( static_cast<size_t>( FacesHeader.DataCount ) );

    for( size_t i=0; i<FacesHeader.DataCount; i++ )
    {
        for( size_t j=0; j<3u; j++ )
        {
            newModel.indices[i].vertexes[j] = FacesData[i].WedgeIndex[j];
        }
    }

    for( size_t i=0, i_max=static_cast<unsigned>( MaterialsHeader.DataCount ); i<i_max; i++ )
    {
        newModel.materials.emplace_back( QString( "./" ) + QString( MaterialsData[i].MaterialName ) );
    }
    newModel.VAOsize = FacesHeader.DataCount * 3u;
    return newModel;
}
