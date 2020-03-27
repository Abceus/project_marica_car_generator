#include "resources/resource_manager.h"
#include "resources/loaders/texture_loader.h"
#include "resources/loaders/file_loader.h"

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

ResourceSubmanager<QImage> &ResourceManager::getTextureManager()
{
    return m_textureManager;
}

ResourceSubmanager<File> &ResourceManager::getFileManager()
{
    return m_fileManager;
}

ResourceManager::ResourceManager()
    : m_textureManager( ResourceSubmanager<QImage>( new TextureLoader ) )
    , m_fileManager( ResourceSubmanager<File>( new FileLoader ) )
{
}
