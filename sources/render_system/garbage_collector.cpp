#include "render_system/garbage_collector.h"

void GarbageCollector::collect() {
    for(auto resource: garbage) {
        delete resource;
    }
    garbage.clear();

    for(auto resource: textureGarbage) {
        delete resource;
    }
    textureGarbage.clear();
}

void GarbageCollector::addResource(Drawable* resource) {
    garbage.insert(resource);
}

void GarbageCollector::addResource(QOpenGLTexture* resource) {
    textureGarbage.insert(resource);
}