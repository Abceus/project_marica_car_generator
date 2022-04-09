#pragma once

#include <QMainWindow>
#include <QDir>
#include "openglsimulationwidget.h"

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
    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    OpenglSimulationWidget simulationWidget;

    QString lastMeshPath = QDir::currentPath();
    QString lastTexturePath = QDir::currentPath();
    QString lastCollisionPath = QDir::currentPath();
};
