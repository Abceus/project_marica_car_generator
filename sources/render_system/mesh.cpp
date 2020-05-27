#include <memory>
#include <algorithm>

#include <QFile>

#include "resources/resource_manager.h"
#include "render_system/mesh.h"

Mesh::Mesh()
        : Drawable()
{
}

Mesh::Mesh( ResourcePointer<Model> model )
{
    m_submeshes.resize( model->materials.size() );

    QVector<size_t> newVertIndexes;
    newVertIndexes.resize( model->vertices.size() );

    for(size_t i = 0; i < model->vertices.size(); i++)
    {
        const auto& vertex = model->vertices[i];
        auto& submesh = m_submeshes[vertex.MaterialIndex];
        submesh.vertices.append( { QVector3D( vertex.Y, vertex.Z, -vertex.X ), QVector2D( vertex.V, vertex.U ) } );
        newVertIndexes[i] = submesh.vertices.size() - 1;
    }

    for( size_t i = 0; i < model->materials.size(); i++ )
    {
        auto& submeshe = m_submeshes[i];
        submeshe.material.texture = ResourceManager::Instance().getTextureManager().get(model->materials[i]);
        submeshe.drawType = DrawType::DT_Mesh;
        submeshe.elementType = ElementsType::PT_Vertex;
    }

    for( size_t i = 0; i < model->indices.size(); i++ )
    {
        auto materialIndex = static_cast<size_t>( model->vertices[model->indices[i].vertexes[0]].MaterialIndex );
        for( auto j = 0; j < 3; j++ )
        {
            auto& submesh = m_submeshes[materialIndex];
            submesh.indices.append( newVertIndexes[model->indices[i].vertexes[j]] );
        }
    }
}

QVector<DrawBuffer> Mesh::getDrawBuffer() const
{
    return m_submeshes;
}

void Mesh::setMaterial( size_t index, Material newMaterial )
{
    if(index < m_submeshes.size())
    {
        m_submeshes[index].material = newMaterial;
    }
}
