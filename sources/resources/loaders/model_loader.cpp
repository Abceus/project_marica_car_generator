#include "resources/loaders/model_loader.h"


Model *ModelLoader::load(const QString &path)
{
    auto model = Model::readPSK(path);
    if(model)
    {
        return new Model( model.value() );
    }
    return nullptr;
}

void ModelLoader::remove(Model *path)
{

}
