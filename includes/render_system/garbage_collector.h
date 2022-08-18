// TODO!: remove qt, rework
#pragma once
#include <QObject>
#include <QOpenGLTexture>
#include <set>
#include "render_system/drawable.h"

class GarbageCollector 
{
public:
    void collect();

    void addResource(Drawable* resource);
    void addResource(QOpenGLTexture* resource); // TODO: rework
private:
    std::set<Drawable*> garbage;
    std::set<QOpenGLTexture*> textureGarbage;
};