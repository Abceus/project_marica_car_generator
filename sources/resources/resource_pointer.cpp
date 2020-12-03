#include "resources/resource_pointer.h"

Counter::Counter(bool hasMaster)
    : m_createFinished(!hasMaster)
    , m_hasMaster(hasMaster)
    , m_count(1)
{

}

Counter Counter::operator++(int)
{
    m_count++;
    if( !m_createFinished && m_count == 2 )
    {
        m_createFinished = true;
    }
    return *this;
}

Counter Counter::operator--(int)
{
    m_count--;
    return *this;
}

bool Counter::operator==(int other) const
{
    return m_count == other;
}

bool Counter::needRemove() const
{
    return m_createFinished && ((m_hasMaster && m_count == 1) || (!m_hasMaster && m_count == 0));
}

void Counter::unsetMaster()
{
    m_hasMaster = false;
}
