#include "resources/resource_manager.h"
#include "resources/loaders/image_loader.h"
#include "resources/loaders/file_loader.h"
#include "resources/loaders/model_loader.h"
#include "resources/loaders/texture_loader.h"

ResourceManager& ResourceManager::Instance()
{
    static ResourceManager instance;
    return instance;
}

ResourceSubmanager<QImage> &ResourceManager::getImageManager()
{
    return m_imageManager;
}

ResourceSubmanager<File> &ResourceManager::getFileManager()
{
    return m_fileManager;
}

ResourceSubmanager<Model> &ResourceManager::getModelManager()
{
    return m_modelManager;
}

ResourceSubmanager<QOpenGLTexture> &ResourceManager::getTextureManager()
{
    return m_textureManager;
}

ResourceManager::ResourceManager()
    : m_imageManager( ResourceSubmanager<QImage>( new ImageLoader ) )
    , m_fileManager( ResourceSubmanager<File>( new FileLoader ) )
    , m_modelManager( ResourceSubmanager<Model>( new ModelLoader ) )
    , m_textureManager( ResourceSubmanager<QOpenGLTexture>( new TextureLoader ) )
{
}
