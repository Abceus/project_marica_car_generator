#ifndef EDIT_WIDGET_H
#define EDIT_WIDGET_H

#include <QWidget>
#include "project_model.h"

namespace Ui {
class EditWidget;
}

class EditWidget : public QWidget
{
    Q_OBJECT

public:
    explicit EditWidget(QWidget *parent = nullptr);
    void setModel( QSharedPointer<ProjectModel> model );
    ~EditWidget();

public slots:
    void clearSkin();
    void changeSkin( size_t index, const QString& path );
    void removeSkin( size_t index );
    void resizeSkin( size_t size );
    void addSkin();
    void meshChange( const QString& path );
    void bodyChange( const QString& path );
    void tireChange( const QString& path );

private slots:
    void skinOpenButton_clicked(int i);

private slots:
    void on_wheelSteerAlongSpinBox_valueChanged(double arg1);

    void on_meshOpenButton_clicked();

    void on_collisionOpenButton_clicked();

    void on_tireColiisionOpenButton_clicked();

private:
    Ui::EditWidget *ui;
    QSharedPointer<ProjectModel> m_model;
};

#endif // EDIT_WIDGET_H
