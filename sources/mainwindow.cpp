#include <QFileDialog>
#include <QDir>
#include <QSharedPointer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resources/model.h"
#include "object.h"
#include "render_system/scene_node.h"
#include "physics/physobject.h"

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
    QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
                                                    QDir::currentPath(),
                                                    tr( "PSK (*.psk)" ) );

    if( fileName != "" )
    {
        auto model = Model::readPSK( fileName );
        size_t textureSize = model.materials.size();

        auto node = ui->mainOpenGLWidget->getScene()->addNode( QSharedPointer<SceneNode>( new SceneNode ) );
        auto drawable = node->addDrawable( ui->mainOpenGLWidget->getRenderer().makeDrawable<Mesh>( Model::readPSK( fileName ) ) );

        ui->mainOpenGLWidget->setBodyObject( QSharedPointer<Object>( new Object( drawable.staticCast<Mesh>(), node ) ) );

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
    if( simulationWidget.isHidden() )
    {
        simulationWidget.show();
        simulationWidget.rewriteThisShit( QString(
                "/home/jamil/gits/ProjectMaricaCarGenerator/example/MaricaFlatoutCollision/StaticMesh/pm_speedevil_tireKColl.psk" ));
        hide();
    }
}


void MainWindow::on_tireColiisionOpenButton_clicked()
{
}

void MainWindow::skinOpenButton_clicked(int i)
{
    if( ui->mainOpenGLWidget->getBodyObject() != nullptr )
    {
        QString fileName = QFileDialog::getOpenFileName( this, tr( "Open File" ),
                                                        QDir::currentPath(),
                                                        tr( "Image (*.png *.xpm *.jpg)" ) );

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
