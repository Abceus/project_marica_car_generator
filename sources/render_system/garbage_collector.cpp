#include "render_system/garbage_collector.h"

GarbageCollector::~GarbageCollector() {
    collect();
}

void GarbageCollector::collect() {
    for (auto resource : garbage) {
        delete resource;
    }
    garbage.clear();

    for (auto resource : textureGarbage) {
        delete resource;
    }
    textureGarbage.clear();
}

void GarbageCollector::addResource(Drawable* resource) {
    garbage.insert(resource);
}

void GarbageCollector::addResource(Texture* resource) {
    textureGarbage.insert(resource);
}