#pragma once

#include <QVector3D>
#include <QVector>
#include <QSharedPointer>
#include <QOpenGLShaderProgram>
#include <QObject>

#include "drawable.h"`

class SceneNode : public QObject
{
    Q_OBJECT
public:
    SceneNode();

    QVector3D getLocation() const;
    void setLocation( const QVector3D& location );

    QVector3D getRotation() const;
    void setRotation( const QVector3D& rotation );

    QVector3D getScale() const;
    void setScale( const QVector3D& scale );
    void setScale( float scale );

    void setParent( SceneNode* parent );

    QSharedPointer<SceneNode> addChild( QSharedPointer<SceneNode> newChild );
    void removeChild( SceneNode* removeChild );

    QSharedPointer<Drawable> addDrawable( QSharedPointer<Drawable> newDrawable );
    void removeDrawable( Drawable* removeDrawable );

    bool isEmpty() const;
    void clear();

    QVector<QSharedPointer<SceneNode>>::ConstIterator begin();
    QVector<QSharedPointer<SceneNode>>::ConstIterator end();

    QVector<QSharedPointer<Drawable>>::ConstIterator drawableBegin();
    QVector<QSharedPointer<Drawable>>::ConstIterator drawableEnd();

    void setShaderProgram( QSharedPointer<QOpenGLShaderProgram> newProgram );
    QSharedPointer<QOpenGLShaderProgram> getShaderProgram() const;

private:
    QVector3D m_location;
    QVector3D m_rotation;
    QVector3D m_scale;
    SceneNode* m_parent;
    QVector<QSharedPointer<SceneNode>> m_childrens;
    QVector<QSharedPointer<Drawable>> m_drawables;
    QSharedPointer<QOpenGLShaderProgram> m_shaderProgram;

signals:
    void drawableAdded( SceneNode* node, Drawable* drawable );
    void drawableRemoved( SceneNode* node, Drawable* drawable );
    void nodeAdded( SceneNode* node );
    void nodeRemoved( SceneNode* node );
};
