#include "render_system/drawable.h"

Drawable::Drawable()
    : m_enable( true )
{

}

void Drawable::setEnable(bool value)
{
    m_enable = value;
}

bool Drawable::getEnable() const
{
    return m_enable;
}
