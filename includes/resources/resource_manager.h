#pragma once

#include <QImage>
#include <QOpenGLTexture>
#include "resources/resource_submanager.h"
#include "resources/file.h"
#include "resources/model.h"

class ResourceManager
{
public:
    static ResourceManager& Instance();
    ResourceManager( ResourceManager const& ) = delete;
    ResourceManager& operator= ( ResourceManager const& ) = delete;

    ResourceSubmanager<QImage>& getImageManager();
    ResourceSubmanager<File>& getFileManager();
    ResourceSubmanager<Model>& getModelManager();
    ResourceSubmanager<QOpenGLTexture>& getTextureManager();
private:
    ResourceManager();
    ~ResourceManager() = default;

    ResourceSubmanager<QImage> m_imageManager;
    ResourceSubmanager<File> m_fileManager;
    ResourceSubmanager<Model> m_modelManager;
    ResourceSubmanager<QOpenGLTexture> m_textureManager;
};
