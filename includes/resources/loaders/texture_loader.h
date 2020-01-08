#pragma once

#include <QImage>

#include "resources/loaders/loader.h"

class TextureLoader : public ILoader<QImage>
{
public:
    QImage* load( const QString& path ) override;
    void remove( QImage* path ) override;
};
