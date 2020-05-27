#pragma once

#include <QMainWindow>
#ifndef WITHOUT_SIMULATION
#include "openglsimulationwidget.h"
#endif
#include "impl_errorsystem.h"
#include "project_model.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow() override;
    void showEvent(QShowEvent *event) override;

private:
    Ui::MainWindow *ui;
#ifndef WITHOUT_SIMULATION
    OpenglSimulationWidget simulationWidget;
#endif
    ImplErrorSystem* m_errorSystem;

    QSharedPointer<ProjectModel> m_model;
};
