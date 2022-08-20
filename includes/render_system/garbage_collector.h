#pragma once
#include <set>
#include "render_system/drawable.h"
#include "render_system/texture.h"

class GarbageCollector 
{
public:
    ~GarbageCollector();
    void collect();

    void addResource(Drawable* resource);
    void addResource(Texture* resource); // TODO: rework
private:
    std::set<Drawable*> garbage;
    std::set<Texture*> textureGarbage;
};