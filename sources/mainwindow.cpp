#include <QFileDialog>
#include <QDir>
#include <QSharedPointer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resources/model.h"
#include "object.h"
#include "render_system/scene_node.h"
#ifndef WITHOUT_SIMULATION
#include "physics/physobject.h"
#endif
#include "render_system/wireframe.h"

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    auto leftSteerWheelLocation = ui->mainOpenGLWidget->getLeftSteerWheel()->getNode()->getOriginLocation();
    auto rightSteerWheelLocation = ui->mainOpenGLWidget->getRightSteerWheel()->getNode()->getOriginLocation();
    auto leftEngWheelLocation = ui->mainOpenGLWidget->getLeftEngWheel()->getNode()->getOriginLocation();
    auto rightEngWheelLocation = ui->mainOpenGLWidget->getRightEngWheel()->getNode()->getOriginLocation();

    auto wheelSteerAlong = ui->wheelSteerAlongSpinBox->value();
    auto wheelSteerAcross = ui->wheelSteerAcrossSpinBox->value();
    auto wheelEngAlong = ui->wheelEngAlongSpinBox->value();
    auto wheelEngAcross = ui->wheelEngAcrossSpinBox->value();
    auto wheelVert = ui->wheelVertSpinBox->value();

    leftSteerWheelLocation.setX( wheelSteerAlong );
    rightSteerWheelLocation.setX( wheelSteerAlong );

    leftEngWheelLocation.setX( wheelEngAlong );
    rightEngWheelLocation.setX( wheelEngAlong );

    leftSteerWheelLocation.setY( wheelSteerAcross );
    rightSteerWheelLocation.setY( -wheelSteerAcross );

    leftEngWheelLocation.setY( wheelEngAcross );
    rightEngWheelLocation.setY( -wheelEngAcross );

    leftSteerWheelLocation.setZ( wheelVert );
    rightSteerWheelLocation.setZ( wheelVert );
    leftEngWheelLocation.setZ( wheelVert );
    rightEngWheelLocation.setZ( wheelVert );

    ui->mainOpenGLWidget->getLeftSteerWheel()->getNode()->setLocation( leftSteerWheelLocation );
    ui->mainOpenGLWidget->getRightSteerWheel()->getNode()->setLocation( rightSteerWheelLocation );
    ui->mainOpenGLWidget->getLeftEngWheel()->getNode()->setLocation( leftEngWheelLocation );
    ui->mainOpenGLWidget->getRightEngWheel()->getNode()->setLocation( rightEngWheelLocation );

#ifndef WITHOUT_SIMULATION
    connect( &simulationWidget, &OpenglSimulationWidget::closed, this, &MainWindow::show );
#endif
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_meshOpenButton_clicked()
{
    QFileDialog fileDialog( this, tr("Open File"), QDir::currentPath(), tr( "PSK (*.psk)" ) );
    fileDialog.setAcceptMode( QFileDialog::AcceptOpen );
    fileDialog.setFileMode( QFileDialog::ExistingFiles );
    fileDialog.setOption( QFileDialog::DontUseNativeDialog, true );

    QString fileName;

    if (QDialog::Accepted == fileDialog.exec())
    {
        fileName = fileDialog.selectedFiles()[0];
    }

    if( fileName != "" )
    {
        auto model = Model::readPSK( fileName );
        size_t textureSize = model.materials.size();

        auto node = ui->mainOpenGLWidget->getBodyObject()->getNode();
//        auto drawable = node->addDrawable( ui->mainOpenGLWidget->getRenderer().makeDrawable<Mesh>( Model::readPSK( fileName ) ) );
        auto drawable = ui->mainOpenGLWidget->getRenderer().makeDrawable<Mesh>( Model::readPSK( fileName ) );

//        node->setLocation( { 0.0f, 100.0f, 0.0f } );

//        node->setRotation( { -10.0f, 0.0f, 0.0f } );
//        node->setRotation( { -90.0f, 0.0f, 0.0f } );
//        node->setRotation( { 0.0f, 0.0f, 90.0f } );

        ui->mainOpenGLWidget->setBodyMesh( drawable.staticCast<Mesh>() );

        if( fileName.size() > 50 )
        {
            fileName = "..." + fileName.right( 47 );
        }
        ui->meshOpenButton->setText( fileName );

        this->clearSkinArrayLayout();

        for( int i = 0; i < textureSize; i++ )
        {
            this->addButtonToArrayLayout();
        }
    }
}

void MainWindow::on_startSimulationButton_clicked()
{
#ifndef WITHOUT_SIMULATION
    if( simulationWidget.isHidden() && ui->mainOpenGLWidget->getBodyObject() )
    {
        simulationWidget.show();
        simulationWidget.prepare( ui->mainOpenGLWidget->getBodyObject()->getDrawable().staticCast<Mesh>()->getModel(), ui->mainOpenGLWidget->getBodyCollisionModel(),
                                  ui->mainOpenGLWidget->getBodyObject()->getNode(), ui->mainOpenGLWidget->getWheelCollisionModel(), ui->mainOpenGLWidget->getLeftSteerWheel()->getNode(),
                                  ui->mainOpenGLWidget->getRightSteerWheel()->getNode(), ui->mainOpenGLWidget->getLeftEngWheel()->getNode(), ui->mainOpenGLWidget->getRightEngWheel()->getNode() );
        hide();
    }
#endif
}


void MainWindow::on_tireColiisionOpenButton_clicked()
{
    QFileDialog fileDialog( this, tr("Open File"), QDir::currentPath(), tr( "PSK (*.psk)" ) );
    fileDialog.setAcceptMode( QFileDialog::AcceptOpen );
    fileDialog.setFileMode( QFileDialog::ExistingFiles );
    fileDialog.setOption( QFileDialog::DontUseNativeDialog, true );

    QString fileName;

    if (QDialog::Accepted == fileDialog.exec())
    {
        fileName = fileDialog.selectedFiles()[0];
    }

    if( fileName != "" )
    {
        ui->mainOpenGLWidget->setWheelCollision( Model::readPSK( fileName ) );

        if( fileName.size() > 50 )
        {
            fileName = "..." + fileName.right( 47 );
        }
        ui->tireColiisionOpenButton->setText( fileName );
    }
}

void MainWindow::skinOpenButton_clicked(int i)
{
    if( ui->mainOpenGLWidget->getBodyObject() )
    {   
        QFileDialog fileDialog( this, tr("Open File"), QDir::currentPath(), tr( "Image (*.bmp *.tga *.dds)" ) );
        fileDialog.setAcceptMode( QFileDialog::AcceptOpen );
        fileDialog.setFileMode( QFileDialog::ExistingFiles );
        fileDialog.setOption( QFileDialog::DontUseNativeDialog, true );

        QString fileName;

        if (QDialog::Accepted == fileDialog.exec())
        {
            fileName = fileDialog.selectedFiles()[0];
        }

        if( fileName != "" )
        {
            ui->mainOpenGLWidget->setBodyTexture( fileName, i );
            if( fileName.size() > 50 )
            {
                fileName = "..." + fileName.right( 47 );
            }

            dynamic_cast<QPushButton*>( ui->skinButtonArrayLayout->itemAt( i + 1 )->widget() )->setText( fileName );
        }
    }
}

void MainWindow::clearSkinArrayLayout()
{
    while( QLayoutItem* item = this->ui->skinButtonArrayLayout->takeAt( 1 ) )
    {
        if( QWidget* widget = item->widget() )
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
    QPushButton *newButton = new QPushButton( "Skin " + QString::number( count ), this->ui->centralWidget );
    newButton->setMinimumSize( 289, 23 );
    newButton->setGeometry( 1, 1, 289, 23 );

    connect( newButton, &QPushButton::released, [=]{ this->skinOpenButton_clicked( count ); } );
    this->ui->skinButtonArrayLayout->addWidget( newButton );
    newButton->show();
    this->ui->skinButtonArrayLayout->update();
}

void MainWindow::on_collisionOpenButton_clicked()
{
    QFileDialog fileDialog( this, tr("Open File"), QDir::currentPath(), tr( "PSK (*.psk)" ) );
    fileDialog.setAcceptMode( QFileDialog::AcceptOpen );
    fileDialog.setFileMode( QFileDialog::ExistingFiles );
    fileDialog.setOption( QFileDialog::DontUseNativeDialog, true );

    QString fileName;

    if (QDialog::Accepted == fileDialog.exec())
    {
        fileName = fileDialog.selectedFiles()[0];
    }

    if( fileName != "" )
    {
        auto model = Model::readPSK( fileName );
        ui->mainOpenGLWidget->getCollisionBodyObject()->setDrawable( ui->mainOpenGLWidget->getRenderer().makeDrawable<WireframeMesh>( model, QColor( 0, 255, 0 ) ) );

        if( fileName.size() > 50 )
        {
            fileName = "..." + fileName.right( 47 );
        }
        ui->collisionOpenButton->setText( fileName );
    }
}

void MainWindow::on_wheelSteerAlongSpinBox_valueChanged(double arg1)
{
    auto leftSteerWheelLocation = ui->mainOpenGLWidget->getLeftSteerWheel()->getNode()->getOriginLocation();
    auto rightSteerWheelLocation = ui->mainOpenGLWidget->getRightSteerWheel()->getNode()->getOriginLocation();
    leftSteerWheelLocation.setX( arg1 );
    rightSteerWheelLocation.setX( arg1 );
    ui->mainOpenGLWidget->getLeftSteerWheel()->getNode()->setLocation( leftSteerWheelLocation );
    ui->mainOpenGLWidget->getRightSteerWheel()->getNode()->setLocation( rightSteerWheelLocation );
}

void MainWindow::on_wheelSteerAcrossSpinBox_valueChanged(double arg1)
{
    auto leftSteerWheelLocation = ui->mainOpenGLWidget->getLeftSteerWheel()->getNode()->getOriginLocation();
    auto rightSteerWheelLocation = ui->mainOpenGLWidget->getRightSteerWheel()->getNode()->getOriginLocation();
    leftSteerWheelLocation.setY( arg1 );
    rightSteerWheelLocation.setY( -arg1 );
    ui->mainOpenGLWidget->getLeftSteerWheel()->getNode()->setLocation( leftSteerWheelLocation );
    ui->mainOpenGLWidget->getRightSteerWheel()->getNode()->setLocation( rightSteerWheelLocation );
}

void MainWindow::on_wheelEngAlongSpinBox_valueChanged(double arg1)
{
    auto leftEngWheelLocation = ui->mainOpenGLWidget->getLeftEngWheel()->getNode()->getOriginLocation();
    auto rightEngWheelLocation = ui->mainOpenGLWidget->getRightEngWheel()->getNode()->getOriginLocation();
    leftEngWheelLocation.setX( arg1 );
    rightEngWheelLocation.setX( arg1 );
    ui->mainOpenGLWidget->getLeftEngWheel()->getNode()->setLocation( leftEngWheelLocation );
    ui->mainOpenGLWidget->getRightEngWheel()->getNode()->setLocation( rightEngWheelLocation );
}

void MainWindow::on_wheelEngAcrossSpinBox_valueChanged(double arg1)
{
    auto leftEngWheelLocation = ui->mainOpenGLWidget->getLeftEngWheel()->getNode()->getOriginLocation();
    auto rightEngWheelLocation = ui->mainOpenGLWidget->getRightEngWheel()->getNode()->getOriginLocation();
    leftEngWheelLocation.setY( arg1 );
    rightEngWheelLocation.setY( -arg1 );
    ui->mainOpenGLWidget->getLeftEngWheel()->getNode()->setLocation( leftEngWheelLocation );
    ui->mainOpenGLWidget->getRightEngWheel()->getNode()->setLocation( rightEngWheelLocation );
}

void MainWindow::on_wheelVertSpinBox_valueChanged(double arg1)
{
    auto leftSteerWheelLocation = ui->mainOpenGLWidget->getLeftSteerWheel()->getNode()->getOriginLocation();
    auto rightSteerWheelLocation = ui->mainOpenGLWidget->getRightSteerWheel()->getNode()->getOriginLocation();
    auto leftEngWheelLocation = ui->mainOpenGLWidget->getLeftEngWheel()->getNode()->getOriginLocation();
    auto rightEngWheelLocation = ui->mainOpenGLWidget->getRightEngWheel()->getNode()->getOriginLocation();
    leftSteerWheelLocation.setZ( arg1 );
    rightSteerWheelLocation.setZ( arg1 );
    leftEngWheelLocation.setZ( arg1 );
    rightEngWheelLocation.setZ( arg1 );
    ui->mainOpenGLWidget->getLeftSteerWheel()->getNode()->setLocation( leftSteerWheelLocation );
    ui->mainOpenGLWidget->getRightSteerWheel()->getNode()->setLocation( rightSteerWheelLocation );
    ui->mainOpenGLWidget->getLeftEngWheel()->getNode()->setLocation( leftEngWheelLocation );
    ui->mainOpenGLWidget->getRightEngWheel()->getNode()->setLocation( rightEngWheelLocation );
}
