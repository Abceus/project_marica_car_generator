#include "resources/resource_manager.h"
#include "resources/image_info.h"

ResourceManager& ResourceManager::Instance() {
    static ResourceManager instance;
    return instance;
}

FileResourceHolder<ImageInfo>& ResourceManager::getImageManager() {
    return imageManager;
}

ResourceManager::ResourceManager() {
    imageManager.setDefaultResource(
        std::make_shared<ImageInfo>("./resources/textures/test.jpg"));
}