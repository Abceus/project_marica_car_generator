#pragma once

#include <QVector>
#include <QSharedPointer>
#include <QOpenGLShaderProgram>
#include <QObject>
#include <QMatrix4x4>

#include "drawable.h"
#include "vector3d.h"

class SceneNode : public Drawable
{
public:
    SceneNode();
    ~SceneNode() = default;

    Vector3D getLocation() override;
    Vector3D getOriginLocation() const override;
    void setLocation( const Vector3D& location );

    Vector3D getRotation() const override;
    Vector3D getOriginRotation() const override;
//    Vector3D getParentRotation() const;
    void setRotation( const Vector3D& rotation );

    Scale3D getScale() const override;
    Scale3D getOriginScale() const override;
    void setScale( const Scale3D& scale );
    void setScale( float scale );

    void setParent( SceneNode* parent ) override;

    QSharedPointer<Drawable> addChild( QSharedPointer<Drawable> newChild );
    void removeChild( Drawable* removedChild );
    void removeChild( QSharedPointer<Drawable> removedChild );

    bool isEmpty() const;
    void clear();

    QVector<QSharedPointer<Drawable>>::ConstIterator begin();
    QVector<QSharedPointer<Drawable>>::ConstIterator end();

    QMatrix4x4 getOriginMatrix() const override;
    QMatrix4x4 getMatrix() const override;
    
    DrawBuffers getDrawBuffers() const override;

    void updateMatrix();
    void updateBuffer();

private:
    Vector3D m_location;
    Vector3D m_rotation;
    Scale3D m_scale;
    QVector<QSharedPointer<Drawable>> m_childrens;
    mutable QMatrix4x4 m_cachedMatrix;
    mutable DrawBuffers m_cachedBuffer;
    mutable bool m_needMatrixUpdate;
    mutable bool m_needBufferUpdate;
};
