#include "resources/resource_pointer.h"

Counter::Counter()
    : m_createFinished(false)
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

bool Counter::needRemove() const
{
    return m_createFinished && m_count == 1;
}
