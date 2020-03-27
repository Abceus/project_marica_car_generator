#pragma once

#include <QImage>
#include "resources/resource_submanager.h"
#include "resources/file.h"

class ResourceManager
{
public:
    static ResourceManager& Instance();
    ResourceManager( ResourceManager const& ) = delete;
    ResourceManager& operator= ( ResourceManager const& ) = delete;

    ResourceSubmanager<QImage>& getTextureManager();
    ResourceSubmanager<File>& getFileManager();
private:
    ResourceManager();
    ~ResourceManager() = default;

    ResourceSubmanager<QImage> m_textureManager;
    ResourceSubmanager<File> m_fileManager;
};
