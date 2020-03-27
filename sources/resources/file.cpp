#include "resources/file.h"


File::File(const QString& path, QByteArray buffer)
    : m_path(path), m_buffer(std::move(buffer))
{

}

QString File::getPath() const
{
    return m_path.absoluteFilePath();
}

QString File::getDirectory() const
{
    return m_path.absolutePath();
}

QString File::getFilename() const
{
    return m_path.fileName();
}

QString File::getExtension() const
{
    return m_path.completeSuffix();
}

QByteArray File::getBuffer() const
{
    return m_buffer;
}
