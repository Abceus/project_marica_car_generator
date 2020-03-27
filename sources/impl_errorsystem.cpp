#include "impl_errorsystem.h"
#include <QDebug>
#include <QString>
#include <QMessageBox>

ImplErrorSystem::ImplErrorSystem(QWidget *parent)
    : m_parent(parent)
{

}

void ImplErrorSystem::active()
{
    m_updateTimer.start(500);
    m_updateTimer.callOnTimeout([this]{
        update();
    });
}

void ImplErrorSystem::deactive()
{
    m_updateTimer.stop();
}

bool ImplErrorSystem::actived() const
{
    return m_updateTimer.isActive();
}

void ImplErrorSystem::update()
{
    for(auto& error: m_errors)
    {
        if(error.critical)
        {
            QMessageBox::critical(m_parent, "Error", QString::fromStdString(error.message));
            if(m_criticalCallback)
            {
               m_criticalCallback();
            }
        } else
        {
            QMessageBox::warning(m_parent, "Error", QString::fromStdString(error.message));
        }
    }
    m_errors.clear();
}

void ImplErrorSystem::setCriticalCallback(std::function<void ()> newCallback)
{
    m_criticalCallback = std::move(newCallback);
}

void ImplErrorSystem::showErrorImpl(std::string error, bool critical)
{
    m_errors.push_back({error, critical});
}
