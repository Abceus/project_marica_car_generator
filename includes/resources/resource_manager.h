#pragma once

#include <QImage>
#include "resources/resource_submanager.h"

class ResourceManager
{
public:
    static ResourceManager& Instance();
    ResourceManager( ResourceManager const& ) = delete;
    ResourceManager& operator= ( ResourceManager const& ) = delete;

    ResourceSubmanager<QImage>& getTextureManager();
private:
    ResourceManager();
    ~ResourceManager() = default;

    ResourceSubmanager<QImage> m_textureManager;
};
