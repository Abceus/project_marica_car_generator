#include "edit_widget.h"
#include "ui_edit_widget.h"
#include <QFileDialog>

EditWidget::EditWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EditWidget)
{
    ui->setupUi(this);
}

EditWidget::~EditWidget()
{
    delete ui;
}

void EditWidget::setModel( QSharedPointer<ProjectModel> model )
{
    if(m_model != model)
    {
        if(m_model)
        {
            disconnect(m_model.get(), &ProjectModel::bodyPathMeshChanged, this, &EditWidget::meshChange);
            disconnect(m_model.get(), &ProjectModel::bodyPathCollisionChanged, this, &EditWidget::bodyChange);
            disconnect(m_model.get(), &ProjectModel::tirePathCollisionChanged, this, &EditWidget::tireChange);
            disconnect(m_model.get(), &ProjectModel::skinCleared, this, &EditWidget::clearSkin);
            disconnect(m_model.get(), &ProjectModel::skinRemoved, this, &EditWidget::removeSkin);
            disconnect(m_model.get(), &ProjectModel::skinAdded, this, &EditWidget::addSkin);
            disconnect(m_model.get(), &ProjectModel::skinPathChanged, this, &EditWidget::changeSkin);
            disconnect(m_model.get(), &ProjectModel::skinResized, this, &EditWidget::resizeSkin);
        }

        m_model = model;

        connect(m_model.get(), &ProjectModel::bodyPathMeshChanged, this, &EditWidget::meshChange);
        connect(m_model.get(), &ProjectModel::bodyPathCollisionChanged, this, &EditWidget::bodyChange);
        connect(m_model.get(), &ProjectModel::tirePathCollisionChanged, this, &EditWidget::tireChange);
        connect(m_model.get(), &ProjectModel::skinCleared, this, &EditWidget::clearSkin);
        connect(m_model.get(), &ProjectModel::skinRemoved, this, &EditWidget::removeSkin);
        connect(m_model.get(), &ProjectModel::skinAdded, this, &EditWidget::addSkin);
        connect(m_model.get(), &ProjectModel::skinPathChanged, this, &EditWidget::changeSkin);
        connect(m_model.get(), &ProjectModel::skinResized, this, &EditWidget::resizeSkin);

        auto projectConfig = m_model->getProjectConfig();

        meshChange(projectConfig.meshPath);
        bodyChange(projectConfig.bodyCollisionPath);
        tireChange(projectConfig.tireCollosionPath);

        clearSkin();


//        QString meshPath;
//        QString bodyCollisionPath;
//        QString tireCollosionPath;
//        QVector<QString> skins;
//        double wheelSteerAlong;
//        double wheelSteerAcross;
//        double wheelEngAlong;
//        double wheelEngAcross;
//        double wheelsVertical;
    }
}

void EditWidget::clearSkin()
{
    while( QLayoutItem* item = this->ui->skinButtonArrayLayout->takeAt( 0 ) )
    {
        if( QWidget* widget = item->widget() )
        {
            widget->deleteLater();
        }
        delete item;
    }
}

void EditWidget::changeSkin( size_t index, const QString& path )
{
    auto normalPath = path;
    if( normalPath.size() > 50 )
    {
        normalPath = "..." + normalPath.right( 47 );
    }

    dynamic_cast<QPushButton*>( ui->skinButtonArrayLayout->itemAt( index )->widget() )->setText( normalPath );
}

void EditWidget::removeSkin( size_t index )
{
    QLayoutItem* item = this->ui->skinButtonArrayLayout->takeAt( index );
    if( QWidget* widget = item->widget() )
    {
        widget->deleteLater();
    }
    delete item;
}

void EditWidget::resizeSkin( size_t size )
{
    clearSkin();
    for(size_t i = 0; i < size; i++)
    {
        addSkin();
    }
}

void EditWidget::addSkin()
{
    int count = this->ui->skinButtonArrayLayout->count();
    QPushButton *newButton = new QPushButton( "Skin " + QString::number( count ), this );
    newButton->setMinimumSize( 289, 23 );
    newButton->setGeometry( 1, 1, 289, 23 );

    connect( newButton, &QPushButton::released, [=]{ this->skinOpenButton_clicked( count ); } );
    this->ui->skinButtonArrayLayout->addWidget( newButton );
    newButton->show();
    this->ui->skinButtonArrayLayout->update();
}

void EditWidget::meshChange( const QString& path )
{
    auto normalPath = path;
    if( normalPath.size() > 50 )
    {
        normalPath = "..." + normalPath.right( 47 );
    }

    if( normalPath != "" )
    {
        ui->meshOpenButton->setText( normalPath );
    }
    else
    {
        ui->meshOpenButton->setText( "Select file" );
    }
}

void EditWidget::bodyChange( const QString& path )
{
    auto normalPath = path;
    if( normalPath.size() > 50 )
    {
        normalPath = "..." + normalPath.right( 47 );
    }

    if( normalPath != "" )
    {
        ui->collisionOpenButton->setText( normalPath );
    }
    else
    {
        ui->collisionOpenButton->setText( "Select file" );
    }
}

void EditWidget::tireChange( const QString& path )
{
    auto normalPath = path;
    if( normalPath.size() > 50 )
    {
        normalPath = "..." + normalPath.right( 47 );
    }

    if( normalPath != "" )
    {
        ui->tireColiisionOpenButton->setText( normalPath );
    }
    else
    {
        ui->tireColiisionOpenButton->setText( "Select file" );
    }
}

void EditWidget::skinOpenButton_clicked(int i)
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
       if( m_model )
       {
           m_model->changeSkin( i, fileName );
       }
    }
}

void EditWidget::on_wheelSteerAlongSpinBox_valueChanged(double arg1)
{
    if(m_model)
    {
        m_model->changeWheelSteerAlong( arg1 );
    }
}

void EditWidget::on_meshOpenButton_clicked()
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
        if(m_model)
        {
            m_model->changeBodyMesh( fileName );
        }
    }
}

void EditWidget::on_collisionOpenButton_clicked()
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
        if(m_model)
        {
            m_model->changeBodyCollision( fileName );
        }
    }
}

void EditWidget::on_tireColiisionOpenButton_clicked()
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
        if(m_model)
        {
            m_model->changeTireCollision( fileName );
        }
    }
}
