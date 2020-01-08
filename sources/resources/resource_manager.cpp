#include "resources/resource_manager.h"
#include "resources/loaders/texture_loader.h"

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

ResourceSubmanager<QImage> &ResourceManager::getTextureManager()
{
    return m_textureManager;
}

ResourceManager::ResourceManager()
    : m_textureManager( ResourceSubmanager<QImage>( new TextureLoader ) )
{
}
