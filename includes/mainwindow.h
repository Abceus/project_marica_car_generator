#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;
    void clearSkinArrayLayout();

    void addButtonToArrayLayout();

private slots:
    void on_meshOpenButton_clicked();
    void on_startSimulationButton_clicked();

    void on_skinOpenButton_clicked(int i);

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
