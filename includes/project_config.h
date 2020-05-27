#pragma once

#include <QString>
#include <QVector>

struct ProjectConfig
{
    QString meshPath;
    QString bodyCollisionPath;
    QString tireCollosionPath;
    QVector<QString> skins;
    double wheelSteerAlong;
    double wheelSteerAcross;
    double wheelEngAlong;
    double wheelEngAcross;
    double wheelsVertical;
};
