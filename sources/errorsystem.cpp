#include "errorsystem.h"

std::unique_ptr<ErrorSystem> ErrorSystem::m_system = nullptr;

void ErrorSystem::showError(std::string error, bool critical)
{
    if(m_system)
    {
        m_system->showErrorImpl(error, critical);
    }
}
