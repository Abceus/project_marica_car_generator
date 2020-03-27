#include "resources/loaders/file_loader.h"

File *FileLoader::load(const QString &path)
{
    QFile file(path);
    if ( !file.open( QIODevice::ReadOnly ) )
        return nullptr;
    return new File(path, file.readAll());
}

void FileLoader::remove(File *path)
{

}
