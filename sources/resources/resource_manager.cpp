#include "resources/resource_manager.h"

ResourceManager& ResourceManager::Instance() {
    static ResourceManager instance;
    return instance;
}

// FileResourceHolder<wxImage>& ResourceManager::getImageManager() {
//     return imageManager;
// }

ResourceManager::ResourceManager() {
    //     imageManager.setDefaultResource(
    //         std::make_shared<wxImage>("./resources/textures/test.jpg"));
}