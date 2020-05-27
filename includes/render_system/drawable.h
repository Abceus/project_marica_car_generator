#pragma once

#include "resources/model.h"
#include "render_system/batching.h"
#include <QVector>


class Drawable
{
public:
    Drawable();
    virtual ~Drawable() = default;
    virtual QVector<DrawBuffer> getDrawBuffer() const = 0;
    void setEnable( bool value );
    bool getEnable() const;
protected:
    bool m_enable;
};
