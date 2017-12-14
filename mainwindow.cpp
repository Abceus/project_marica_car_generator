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

    if(fileName != "")
    {
        std::unique_ptr<Model> newModel = ui->mainOpenGLWidget->getModel(fileName);
        int textureSize = newModel->getTexturesSize();
        ui->mainOpenGLWidget->setBodyObject(new Object(std::move(newModel), 0, 0, 0));

        if(fileName.size() > 50)
        {
            fileName = "..." + fileName.right(47);
        }
        ui->meshOpenButton->setText(fileName);

        this->clearSkinArrayLayout();

        for(int i = 0; i < textureSize; i++)
        {
            this->addButtonToArrayLayout();
        }
    }
}

void MainWindow::on_skinOpenButton_clicked(int i)
{
    if(ui->mainOpenGLWidget->getBodyObject() != nullptr)
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                        QDir::currentPath(),
                                                        tr("Image (*.png *.xpm *.jpg)"));

        if(fileName != "")
        {
            ui->mainOpenGLWidget->setBodyTexture(fileName, i);
            if(fileName.size() > 50)
            {
                fileName = "..." + fileName.right(47);
            }

            ((QPushButton*)(ui->skinButtonArrayLayout->itemAt(i+1)->widget()))->setText(fileName);
        }
    }
}

void MainWindow::clearSkinArrayLayout()
{
    while (QLayoutItem* item = this->ui->skinButtonArrayLayout->takeAt(1))
    {
        if (QWidget* widget = item->widget())
        {
            widget->deleteLater();
        }
        delete item;
    }
    this->ui->emptySkinOpenButton->show();
}

void MainWindow::addButtonToArrayLayout()
{
    this->ui->emptySkinOpenButton->hide();
    int count = this->ui->skinButtonArrayLayout->count() - 1;
    QPushButton *newButton = new QPushButton("Skin " + QString::number(count), this->ui->centralWidget);
    newButton->setMinimumSize(289, 23);
    newButton->setGeometry(1, 1, 289, 23);

    connect(newButton, QPushButton::released, [=]{this->on_skinOpenButton_clicked(count);});
    this->ui->skinButtonArrayLayout->addWidget(newButton);
    newButton->show();
    this->ui->skinButtonArrayLayout->update();
}
