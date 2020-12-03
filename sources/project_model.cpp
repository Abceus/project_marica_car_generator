#include "project_model.h"
#include "resources/resource_manager.h"


void ProjectModel::init( const ProjectConfig& newConfig )
{
    clearSkin();
    m_projectConfig = ProjectConfig();

    changeBodyMesh( newConfig.meshPath );
    changeBodyCollision( newConfig.bodyCollisionPath );
    changeTireCollision( newConfig.tireCollosionPath );

    for(size_t i = 0; i < newConfig.skins.size(); i++ )
    {
        addSkin();
        changeSkin( i, newConfig.skins[i] );
    }

    changeWheelSteerAlong( newConfig.wheelSteerAlong );
    changeWheelSteerAcross( newConfig.wheelSteerAcross );
    changeWheelEngAlong( newConfig.wheelEngAlong );
    changeWheelEngAcross( newConfig.wheelEngAcross );
    changeWheelsVertical( newConfig.wheelsVertical );
}

void ProjectModel::changeBodyMesh( const QString& path )
{
    if( path == m_projectConfig.meshPath) return;

    auto model = ResourceManager::Instance().getModelManager().get(path);
    if( model )
    {
        m_bodyModel = model;

        m_projectConfig.meshPath = path;

        emit bodyMeshChanged( m_bodyModel );
        emit bodyPathMeshChanged( m_projectConfig.meshPath );

        resizeSkin( m_bodyModel->materials.size() );
    }
}

void ProjectModel::changeBodyCollision( const QString& path )
{
    if( path == m_projectConfig.bodyCollisionPath ) return;

    auto model = CollisionObject::readASE(path);
    if( model )
    {
        m_bodyCollision = model.value();

        m_projectConfig.bodyCollisionPath = path;

        emit bodyCollisionChanged( m_bodyCollision );
        emit bodyPathCollisionChanged( m_projectConfig.bodyCollisionPath );
    }
}

void ProjectModel::changeTireCollision( const QString& path )
{
    if( path == m_projectConfig.tireCollosionPath ) return;

    auto model = CollisionObject::readASE(path);
    if( model )
    {
        m_tireCollision = model.value();

        m_projectConfig.tireCollosionPath = path;

        emit tireCollisionChanged( m_tireCollision );
        emit tirePathCollisionChanged( m_projectConfig.tireCollosionPath );
    }
}

void ProjectModel::removeSkin( size_t index )
{
    if( index < m_skins.size() )
    {
        m_skins.remove( index );
        m_projectConfig.skins.remove( index );

        emit skinRemoved( index );
    }
}

void ProjectModel::addSkin()
{
    m_skins.push_back( nullptr );
    emit skinAdded();
}

void ProjectModel::changeSkin( size_t index, const QString& path )
{
    if( index < m_projectConfig.skins.size() && m_projectConfig.skins[index] != path )
    {
        auto skin = ResourceManager::Instance().getImageManager().get( path );
        if( skin )
        {
            m_skins[index] = skin;
            m_projectConfig.skins[index] = path;
            emit skinChanged(index, skin);
            emit skinPathChanged(index, path);
        }
    }
}

void ProjectModel::clearSkin()
{
    m_skins.clear();
    m_projectConfig.skins.clear();
    emit skinCleared();
}

void ProjectModel::resizeSkin( size_t size )
{
    m_skins.resize(size);
    m_projectConfig.skins.resize(size);
    emit skinResized(size);
}

void ProjectModel::changeWheelSteerAlong( double value )
{
    if( m_projectConfig.wheelSteerAlong != value )
    {
        m_projectConfig.wheelSteerAlong = value;

        emit wheelSteerAlongChanged( value );
    }
}

void ProjectModel::changeWheelSteerAcross( double value )
{
    if( m_projectConfig.wheelSteerAcross != value )
    {
        m_projectConfig.wheelSteerAcross = value;

        emit wheelSteerAcrossChanged( value );
    }
}

void ProjectModel::changeWheelEngAlong( double value )
{
    if( m_projectConfig.wheelEngAlong != value )
    {
        m_projectConfig.wheelEngAlong = value;

        emit wheelEngAlongChanged( value );
    }
}

void ProjectModel::changeWheelEngAcross( double value )
{
    if( m_projectConfig.wheelEngAcross != value )
    {
        m_projectConfig.wheelEngAcross = value;

        emit wheelEngAcrossChanged( value );
    }
}

void ProjectModel::changeWheelsVertical( double value )
{
    if( m_projectConfig.wheelsVertical != value )
    {
        m_projectConfig.wheelsVertical = value;

        emit wheelsVerticalChanged( value );
    }
}

ProjectConfig ProjectModel::getProjectConfig() const
{
    return m_projectConfig;
}
