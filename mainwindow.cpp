#include <QFileDialog>
#include <QDir>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "model.h"
#include "object.h"

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

void MainWindow::on_meshOpenButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                    QDir::currentPath(),
                                                    tr("PSK (*.psk)"));
    //Model test(fileName);
    if(fileName != "")
    {
        std::unique_ptr<Model> test = ui->mainOpenGLWidget->getModel(fileName);
        ui->mainOpenGLWidget->setBodyObject(new Object(std::move(test), 0, 0, 0));
        //ui->mainOpenGLWidget->setBodyModel(std::move(test));
        ui->meshOpenButton->setText(fileName);
    }
}

void MainWindow::on_skinOpenButton_clicked()
{
    if(ui->mainOpenGLWidget->getBodyObject() != nullptr)
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QDir::currentPath(),
                                                        tr("Image (*.png *.xpm *.jpg *.tga)"));

        if(fileName != "")
        {
            ui->mainOpenGLWidget->setBodyTexture(fileName);
            ui->skinOpenButton->setText(fileName);
        }
    }
}
