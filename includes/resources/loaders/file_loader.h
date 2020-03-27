#pragma once

#include "resources/loaders/loader.h"
#include "resources/file.h"

class FileLoader : public ILoader<File>
{
public:
    File* load( const QString& path ) override;
    void remove( File* path ) override;
};
