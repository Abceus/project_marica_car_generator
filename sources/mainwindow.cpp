#include <QFileDialog>
#include <QSharedPointer>
#include <qfileinfo.h>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resources/model.h"
#include "object.h"
#include "render_system/scene_node.h"
#include "physics/physobject.h"
#include "render_system/wireframe.h"

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
{
    ui->setupUi( this );

    connect( &simulationWidget, &OpenglSimulationWidget::closed, this, &MainWindow::show );
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_meshOpenButton_clicked()
{
    //QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
    //                                                QDir::currentPath(),
    //                                                tr( "PSK (*.psk)" ) );

    QFileDialog fileDialog( this, tr("Open File"), lastMeshPath, tr( "PSK (*.psk)" ) );
    fileDialog.setAcceptMode( QFileDialog::AcceptOpen );
    fileDialog.setFileMode( QFileDialog::ExistingFiles );
    fileDialog.setOption( QFileDialog::DontUseNativeDialog, true );

    //QStringList fileNames = fileDialog.selectedFiles();

    QString fileName;// = fileNames.empty() ? "" : fileNames[0];

    if (QDialog::Accepted == fileDialog.exec())
    {
        fileName = fileDialog.selectedFiles()[0];
    }

    if( !fileName.isEmpty() )
    {
        auto model = Model::readPSK( fileName );
        size_t textureSize = model.materials.size();

        auto node = ui->mainOpenGLWidget->getBodyObject()->getNode();
        auto drawable = node->addDrawable( ui->mainOpenGLWidget->getRenderer().makeDrawable<Mesh>( Model::readPSK( fileName ) ) );

        node->setRotation( { 0.0f, 0.0f, 45.0f } );

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

        lastMeshPath = QFileInfo(fileName).absoluteDir().path();
    }
}

void MainWindow::on_startSimulationButton_clicked()
{
    if( simulationWidget.isHidden() && ui->mainOpenGLWidget->getBodyObject() )
    {
        simulationWidget.show();
        simulationWidget.prepare( ui->mainOpenGLWidget->getBodyObject()->getDrawable().staticCast<Mesh>()->getModel(), ui->mainOpenGLWidget->getBodyCollisionModel(), ui->mainOpenGLWidget->getBodyObject()->getNode() );
        hide();
    }
}


void MainWindow::on_tireColiisionOpenButton_clicked()
{
}

void MainWindow::skinOpenButton_clicked(int i)
{
    if( ui->mainOpenGLWidget->getBodyObject() )
    {
        QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
                                                        lastTexturePath,
                                                        tr( "Image (*.png *.xpm *.jpg)" ) );

        if( fileName != "" )
        {
            ui->mainOpenGLWidget->setBodyTexture( fileName, i );
            if( fileName.size() > 50 )
            {
                fileName = "..." + fileName.right( 47 );
            }

            dynamic_cast<QPushButton*>( ui->skinButtonArrayLayout->itemAt( i + 1 )->widget() )->setText( fileName );
        
            lastTexturePath = QFileInfo(fileName).absoluteDir().path();
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

void MainWindow::on_pushButton_2_clicked()
{
    QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
                                                    lastCollisionPath,
                                                    tr( "PSK (*.psk)" ) );

    if( fileName != "" )
    {
        auto model = Model::readPSK( fileName );
        auto node = ui->mainOpenGLWidget->getCollisionBodyObject()->getNode();
        auto drawable = node->addDrawable( ui->mainOpenGLWidget->getRenderer().makeDrawable<WireframeMesh>( model, QColor( 0, 255, 0 ) ) );

        if( fileName.size() > 50 )
        {
            fileName = "..." + fileName.right( 47 );
        }
        ui->pushButton_2->setText( fileName );

        lastCollisionPath = QFileInfo(fileName).absoluteDir().path();
    }
}
