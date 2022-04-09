#include "resources/resource_manager.h"
#include <QImage>

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

FileResourceHolder<QImage>& ResourceManager::getImageManager() 
{
    return imageManager;
}

ResourceManager::ResourceManager()
{
    imageManager.setDefaultResource(QSharedPointer<QImage>(new QImage("./resources/textures/test.jpg")));
}