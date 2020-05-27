#pragma once

#include <QOpenGLTexture>

#include "resources/loaders/loader.h"

class TextureLoader : public ILoader<QOpenGLTexture>
{
public:
    QOpenGLTexture* load( const QString& path ) override;
    void remove( QOpenGLTexture* object ) override;
};
