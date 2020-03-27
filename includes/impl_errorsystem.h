#pragma once

#include "errorsystem.h"
#include <map>
#include <string>
#include <vector>
#include <QWidget>
#include <QTimer>
#include <functional>

struct Error
{
    std::string message;
    bool critical;
};

class ImplErrorSystem : public ErrorSystem
{
public:
    ImplErrorSystem(QWidget* parent);
    void active();
    void deactive();
    bool actived() const;
    void update();
    void setCriticalCallback(std::function<void()> newCallback);
protected:
    void showErrorImpl(std::string error, bool critical) override;
    QWidget* m_parent;
    std::vector<Error> m_errors;
    QTimer m_updateTimer;
    std::function<void()> m_criticalCallback;
};
