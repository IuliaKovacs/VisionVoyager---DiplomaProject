#include "adminModeWindow.h"
#include "ui_adminmodewindow.h"
#include "cameraWidget.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include <iostream>
#include <filesystem>
#include <cstdio>

namespace fs = std::filesystem;
using namespace std; 

#define ROUTE_DATABASE_PATH_SECTION "../RouteDatabase/"
#define SECTION "Section "
#define FILE_TERMINATION ".txt"

int section_A_routes = 0;
int section_B_routes = 0;
int section_C_routes = 0;

void start_GUI(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AdminModeWindow w;
    w.show();
    a.exec(); 
}

AdminModeWindow::AdminModeWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::AdminModeWindow)
{
    ui->setupUi(this);
    loadFromDatabase();

    ui->routeTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->errorLabel->setVisible(false);

    camera_widget = new CameraWidget(this);
    ui->layout_2->addWidget(camera_widget);

    file_drop_widget_fr = new FileDropWidget(this);
    ui->dragNdropLayout->addWidget(file_drop_widget_fr);

    file_drop_route = new FileDropWidget(this);
    file_drop_route->setAcceptsSingleFile(true);
    ui->routeDragNdropLayout->addWidget(file_drop_route);
    
}

AdminModeWindow::~AdminModeWindow()
{
    delete ui;
}

void AdminModeWindow::on_startRegistrationButton_clicked()
{
    cout << "Start register clicked!" << endl;
    RouteRegistration::set_register_enabled_true();
}


void AdminModeWindow::on_endReigstrationButton_clicked()
{
    cout << "Stop register clicked!" << endl;
    RouteRegistration::end_registration();
}


void AdminModeWindow::on_saveChangesButton_clicked()
{
    cout << "Save Changes clicked!" << endl;
    for(int row = 0; row < ui->routeTableWidget->rowCount(); row++)
    {
        QTableWidgetItem *item = ui->routeTableWidget->item(row, 0);
        if (item) 
        {
            string item_name = (item->text()).toStdString();
            vector<string>& route_paths = RouteRegistration::get_route_paths();
            QString old_name_aux = (QString::fromStdString(route_paths[row])).split('/').last();;
            string old_name = old_name_aux.toStdString();
            if(old_name != (item_name))
            {   
                string new_file_name;
                string old_file_name = route_paths[row] + string(FILE_TERMINATION);

                QTableWidgetItem *section_item = ui->routeTableWidget->item(row, 1);
                if((section_item) && ((section_item->text()).toStdString().find("-") == string::npos))
                {
                    new_file_name = string(ROUTE_DATABASE_PATH_SECTION) + (section_item->text()).toStdString() + "/" + item_name;
                }
                else if(section_item)
                {
                    new_file_name = string(ROUTE_DATABASE_PATH_SECTION) + item_name;  
                }

                route_paths[row] = new_file_name;
                new_file_name += string(FILE_TERMINATION);
                // cout << old_file_name << endl;
                // cout << new_file_name << endl;
                rename(old_file_name.c_str(), new_file_name.c_str());
            }
        }
    }
}


void AdminModeWindow::on_insertRouteButton_clicked()
{
    cout << "Insert Route clicked!" << endl;
    string new_route_name = (ui->routeNameLineEdit->text()).toStdString();
    string new_route_section = (ui->comboBox->currentText()).toStdString();
    
    if(((new_route_section == "Section A") && (section_A_routes == 3))
    || ((new_route_section == "Section B") && (section_B_routes == 3))
    || ((new_route_section == "Section C") && (section_C_routes == 3)))
    {
        ui->errorLabel->setVisible(true);
        ui->errorLabel->setText("Error: The selected section is full! Please delete an entry first!");
    }
    else 
    {
        ui->errorLabel->setVisible(false);
    }


}


void AdminModeWindow::on_deleteRouteButton_clicked()
{
    cout << "Delete Route clicked!" << endl;
    ui->errorLabel->setVisible(false);
    QList<QTableWidgetItem*> selectedItems = ui->routeTableWidget->selectedItems();

    for (QTableWidgetItem* item : selectedItems) 
    {
        int row = item->row();
        
        QTableWidgetItem* route_name_item = ui->routeTableWidget->item(row, 0); 
        QTableWidgetItem* section_item = ui->routeTableWidget->item(row, 1);
        if (route_name_item && section_item) 
        {
            string route_name = (route_name_item->text()).toStdString();
            string section = (section_item->text()).toStdString();

            if(section != " - ")
            {
                string file_path = string(ROUTE_DATABASE_PATH_SECTION) + section + "/" + route_name + string(FILE_TERMINATION);
                cout << file_path << endl;
                remove(file_path.c_str());
            }
            else
            {
                string file_path = string(ROUTE_DATABASE_PATH_SECTION) + route_name + string(FILE_TERMINATION);
                cout << file_path << endl;
                remove(file_path.c_str());
            }

            ui->routeTableWidget->removeRow(row);
        }
        else
        {
            cout << "Error: There is a problem with deleting the route!" << endl;
        }
    }


}

void AdminModeWindow::on_addRecognizedFaceButton_clicked()
{
    cout << "Add New Subject clicked!" << endl;
}

void AdminModeWindow::loadFromDatabase()
{
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

