#include "../CameraModule/person.h"
#include "../CameraModule/cameraModule.h"
#include "adminModeWindow.h"
#include "ui_adminmodewindow.h"



void AdminModeWindow::load_recognized_persons()
{
    while (ui->personsTableWidget->rowCount() > 0) 
    {
        ui->personsTableWidget->removeRow(0);
    }   

    for(auto person : CameraModule::get_recognized_persons())
    {
        int tableRow = ui->personsTableWidget->rowCount();
        ui->personsTableWidget->insertRow(tableRow);

        QTableWidgetItem* item_fn = new QTableWidgetItem(QString::fromStdString(person.get_first_name()));
        item_fn->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item_ln = new QTableWidgetItem(QString::fromStdString(person.get_last_name()));
        item_ln->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem* item_role = new QTableWidgetItem(QString::fromStdString(person.get_role()));
        item_role->setTextAlignment(Qt::AlignCenter);
        ui->personsTableWidget->setItem(tableRow, 0, item_fn);
        ui->personsTableWidget->setItem(tableRow, 1, item_ln);
        ui->personsTableWidget->setItem(tableRow, 2, item_role);
    }
}