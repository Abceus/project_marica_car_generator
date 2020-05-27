#pragma once

#include "resources/model.h"

#include "resources/loaders/loader.h"

class ModelLoader : public ILoader<Model>
{
public:
    Model* load( const QString& path ) override;
    void remove( Model* path ) override;
};
