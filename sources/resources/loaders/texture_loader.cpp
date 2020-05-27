#include "resources/loaders/texture_loader.h"
#include "resources/resource_manager.h"

QOpenGLTexture *TextureLoader::load(const QString &path)
{
    auto image = ResourceManager::Instance().getImageManager().get( path );
    if( !image.get() || image->isNull() )
    {
        image = ResourceManager::Instance().getImageManager().get( "./resources/textures/DefaultTexture.tga" );
    }
    return new QOpenGLTexture( *image.get() );

}

void TextureLoader::remove(QOpenGLTexture *object)
{

}
