#pragma once

#include <QString>
#include <QByteArray>
#include <QFileInfo>

class File
{
public:
    File( const QString& path, QByteArray buffer );
    QString getPath() const;
    QString getDirectory() const;
    QString getFilename() const;
    QString getExtension() const;
    QByteArray getBuffer() const;
private:
    QFileInfo m_path;
    QByteArray m_buffer;
};
