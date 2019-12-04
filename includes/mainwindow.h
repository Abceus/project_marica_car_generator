#pragma once

#include <QMainWindow>
#ifndef WITHOUT_SIMULATION
#include "openglsimulationwidget.h"
#endif

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow( QWidget *parent = nullptr );
    ~MainWindow() override;
    void clearSkinArrayLayout();
    void addButtonToArrayLayout();
private slots:
    void on_meshOpenButton_clicked();
    void on_startSimulationButton_clicked();
    void on_tireColiisionOpenButton_clicked();
    void skinOpenButton_clicked(int i);
    void on_collisionOpenButton_clicked();

    void on_wheelSteerAlongSpinBox_valueChanged(double arg1);

    void on_wheelSteerAcrossSpinBox_valueChanged(double arg1);

    void on_wheelEngAlongSpinBox_valueChanged(double arg1);

    void on_wheelEngAcrossSpinBox_valueChanged(double arg1);

    void on_wheelVertSpinBox_valueChanged(double arg1);

private:
    Ui::MainWindow *ui;
#ifndef WITHOUT_SIMULATION
    OpenglSimulationWidget simulationWidget;
#endif
};
