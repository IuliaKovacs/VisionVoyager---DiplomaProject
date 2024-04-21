#include "../CameraModule/person.h"
#include "../CameraModule/cameraModule.h"
#include "adminModeWindow.h"
#include "ui_adminmodewindow.h"
#include "../RouteRegistration/routeRegistrationUtils.h"


void AdminModeWindow::loadFromDatabase()
{
    while (ui->routeTableWidget->rowCount() > 0) 
    {
        ui->routeTableWidget->removeRow(0);
    }

    section_A_routes = 0;
    section_B_routes = 0;
    section_C_routes = 0;

    for(const string& route : RouteRegistration::get_route_paths())
    {
        QString path = QString::fromStdString(route);
        QStringList files = path.split('/');
        QTableWidgetItem* section;

        if((files.size() >= 2) && (files.at(files.size()-2).contains("Section")))
        {
            section = new QTableWidgetItem(files.at(files.size()-2));
            if(files.at(files.size()-2).contains("A"))
            {
                section_A_routes++;
            }
            else if(files.at(files.size()-2).contains("B"))
            {
                section_B_routes++;
            }
            else if(files.at(files.size()-2).contains("C"))
            {
                section_C_routes++;   
            }
        }
        else
        {
            section = new QTableWidgetItem(QString::fromStdString(" - "));
        }
        section->setTextAlignment(Qt::AlignCenter);

        int tableRow = ui->routeTableWidget->rowCount();
        ui->routeTableWidget->insertRow(tableRow);

        QTableWidgetItem* item = new QTableWidgetItem(files.last());
        item->setTextAlignment(Qt::AlignCenter);
        ui->routeTableWidget->setItem(tableRow, 0, item);
        ui->routeTableWidget->setItem(tableRow, 1, section);
    }


    for(int row = 0; row < ui->routeTableWidget->rowCount(); row++) 
    {
        QTableWidgetItem *item = ui->routeTableWidget->item(row, 1); 
        if(item) 
        {
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        }
    }
}


string AdminModeWindow::get_filename_from_path(string path)
{
    QString qpath = QString::fromStdString(path);
    QStringList fields = qpath.split('/');
    return (fields.last()).toStdString();
}


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