#pragma once

#include <QObject>
#include <QString>
#include "resources/model.h"
#include "resources/resource_pointer.h"
#include "project_config.h"
#include "collision_object.h"

class ProjectModel : public QObject
{
    Q_OBJECT
public:
    void init( const ProjectConfig& newConfig );
    void changeBodyMesh( const QString& path );
    void changeBodyCollision( const QString& path );
    void changeTireCollision( const QString& path );

//    void removeSkin( size_t index );
//    void addSkin();
    void changeSkin( size_t index, const QString& path );
//    void clearSkin();
//    void resizeSkin( size_t size );

    void changeWheelSteerAlong( double value );
    void changeWheelSteerAcross( double value );
    void changeWheelEngAlong( double value );
    void changeWheelEngAcross( double value );
    void changeWheelsVertical( double value );

    ProjectConfig getProjectConfig() const;
signals:
    void bodyMeshChanged( ResourcePointer<Model> model );
    void bodyCollisionChanged( CollisionObject model );
    void tireCollisionChanged( CollisionObject model );

    void bodyPathMeshChanged( const QString& path );
    void bodyPathCollisionChanged( const QString& path );
    void tirePathCollisionChanged( const QString& path );

    void skinRemoved( size_t index );
    void skinAdded();
    void skinChanged( size_t index, ResourcePointer<QImage> skin );
    void skinCleared();
    void skinResized( size_t size );

    void skinPathChanged( size_t index,const QString& path );

    void wheelSteerAlongChanged( double value );
    void wheelSteerAcrossChanged( double value );
    void wheelEngAlongChanged( double value );
    void wheelEngAcrossChanged( double value );
    void wheelsVerticalChanged( double value );
private:
    ResourcePointer<Model> m_bodyModel;
    CollisionObject m_bodyCollision;
    CollisionObject m_tireCollision;

    QVector<ResourcePointer<QImage>> m_skins;

    ProjectConfig m_projectConfig;

    void removeSkin( size_t index );
    void addSkin();
//    void changeSkin( size_t index, const QString& path );
    void clearSkin();
    void resizeSkin( size_t size );
};
