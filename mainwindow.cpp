#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "model.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    "C:/",
                                                    tr("PSK (*.psk)"));
    //Model test(fileName);
    if(fileName != "")
    {
        std::unique_ptr<Model> test = ui->mainOpenGLWidget->getModel(fileName);
        ui->mainOpenGLWidget->setBodyModel(std::move(test));
    }
}
