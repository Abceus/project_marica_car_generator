#include <QFileDialog>
#include <QDir>
#include <QSharedPointer>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "resources/model.h"
#include "resources/resource_manager.h"
#include "object.h"
#include "render_system/scene_node.h"
#ifndef WITHOUT_SIMULATION
#include "physics/physobject.h"
#endif
#include "render_system/wireframe.h"

#include "errorsystem.h"

MainWindow::MainWindow( QWidget *parent )
    : QMainWindow( parent )
    , ui( new Ui::MainWindow )
    , m_errorSystem(dynamic_cast<ImplErrorSystem*>(ErrorSystem::setSystem<ImplErrorSystem>(this)))
    , m_model( new ProjectModel )
{
    ui->setupUi( this );

    ui->settingsWidget->setModel(m_model);
    ui->mainOpenGLWidget->setModel(m_model);

#ifndef WITHOUT_SIMULATION
    connect( &simulationWidget, &OpenglSimulationWidget::closed, this, &MainWindow::show );
#endif

    m_errorSystem->setCriticalCallback([this]{ this->close(); });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showEvent(QShowEvent *event)
{
    QMainWindow::showEvent(event);
    m_errorSystem->active();

#ifndef WITHOUT_SIMULATION
    if(!simulationWidget.isGLinited())
    {
        // Call for init GL
        simulationWidget.show();
        simulationWidget.close();
    }
#endif
}
