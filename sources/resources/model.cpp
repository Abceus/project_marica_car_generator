#include <ios>
#include <memory>
#include <fstream>

#include "resources/model.h"
#include "resources/unanimation.h"

Model Model::readPSK( const std::string &filename )
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

    std::ifstream file( filename, std::ios_base::in | std::ios_base::binary );
    if ( !file.is_open() )
        return newModel;

    file.read( reinterpret_cast<char*>( &GeneralHeader ), sizeof( VChunkHeader ) );
    file.read( reinterpret_cast<char*>( &PointsHeader ), sizeof( VChunkHeader ) );
    PointsData.resize( static_cast<size_t>( PointsHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( PointsData.data() ), PointsHeader.DataSize * PointsHeader.DataCount );

    file.read( reinterpret_cast<char*>( &WedgesHeader ), sizeof( VChunkHeader ) );
    WedgesData.resize( static_cast<size_t>( WedgesHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( WedgesData.data() ), WedgesHeader.DataSize * WedgesHeader.DataCount );

    file.read( reinterpret_cast<char*>( &FacesHeader ), sizeof( VChunkHeader ) );
    FacesData.resize( static_cast<size_t>( FacesHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( FacesData.data() ), FacesHeader.DataSize * FacesHeader.DataCount );

    file.read( reinterpret_cast<char*>( &MaterialsHeader ), sizeof( VChunkHeader ) );
    MaterialsData.resize( static_cast<size_t>( MaterialsHeader.DataCount ) );
    file.read( reinterpret_cast<char*>( MaterialsData.data() ), MaterialsHeader.DataSize * MaterialsHeader.DataCount );

    file.close();

    newModel.vertices.resize( static_cast<size_t>( WedgesHeader.DataCount ) );

    for( size_t i=0; i<WedgesHeader.DataCount; i++ )
    {
        newModel.vertices[i].X = PointsData[WedgesData[i].PointIndex].Y;
        newModel.vertices[i].Y = PointsData[WedgesData[i].PointIndex].Z;
        newModel.vertices[i].Z = -PointsData[WedgesData[i].PointIndex].X;
        newModel.vertices[i].U = WedgesData[i].U;
        newModel.vertices[i].V = WedgesData[i].V;
        newModel.vertices[i].MaterialIndex = WedgesData[i].MatIndex;
    }

    newModel.faces.resize( static_cast<size_t>( FacesHeader.DataCount ) );

    for( size_t i=0; i<FacesHeader.DataCount; i++ )
    {
        for( size_t j=0; j<3u; j++ )
        {
            newModel.faces[i].vertexes[j] = FacesData[i].WedgeIndex[j];
        }
    }

    for( size_t i=0, i_max=static_cast<unsigned>( MaterialsHeader.DataCount ); i<i_max; i++ )
    {
        newModel.materials.emplace_back( std::string( "./" ) + std::string( MaterialsData[i].MaterialName ) );
    }
    return newModel;
}
