#pragma once

#include <QVector>
#include <QSharedPointer>
#include <QOpenGLShaderProgram>
#include <QObject>
#include <QMatrix4x4>

#include "drawable.h"
#include "vector3d.h"

class SceneNode
{
public:
    SceneNode();

    Vector3D getLocation() const;
    Vector3D getOriginLocation() const;
    void setLocation( const Vector3D& location );

    Vector3D getRotation() const;
    Vector3D getOriginRotation() const;
//    Vector3D getParentRotation() const;
    void setRotation( const Vector3D& rotation );

    Scale3D getScale() const;
    void setScale( const Scale3D& scale );
    void setScale( float scale );

    void setParent( SceneNode* parent );

    QSharedPointer<SceneNode> addChild( QSharedPointer<SceneNode> newChild );
    void removeChild( SceneNode* removedChild );
    void removeChild( QSharedPointer<SceneNode> removedChild );

    QSharedPointer<Drawable> addDrawable( QSharedPointer<Drawable> newDrawable );
    void removeDrawable( Drawable* removeDrawable );

    bool isEmpty() const;
    void clear();

    QVector<QSharedPointer<SceneNode>>::ConstIterator begin();
    QVector<QSharedPointer<SceneNode>>::ConstIterator end();

    QVector<QSharedPointer<Drawable>>::ConstIterator drawableBegin();
    QVector<QSharedPointer<Drawable>>::ConstIterator drawableEnd();

    QMatrix4x4 getOriginMatrix() const;
    QMatrix4x4 getMatrix() const;

private:
    Vector3D m_location;
    Vector3D m_rotation;
    Scale3D m_scale;
    SceneNode* m_parent;
    QVector<QSharedPointer<SceneNode>> m_childrens;
    QVector<QSharedPointer<Drawable>> m_drawables;
};
