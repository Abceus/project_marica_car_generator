#pragma once

#include <memory>
#include <string>

class ErrorSystem
{
public:
    template<typename SystemType, typename... Args>
    static ErrorSystem* setSystem(Args... args);

    static void showError(std::string error, bool critical=false);
protected:
    static std::unique_ptr<ErrorSystem> m_system;

    virtual void showErrorImpl(std::string error, bool critical) = 0;
};


template<typename SystemType, typename... Args>
ErrorSystem* ErrorSystem::setSystem(Args... args)
{
    m_system = std::make_unique<SystemType>(args...);
    return m_system.get();
}
