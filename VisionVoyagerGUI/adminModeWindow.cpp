#include "adminModeWindow.h"
#include "ui_adminmodewindow.h"
#include "cameraWidget.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std; 

#define ROUTE_DATABASE_PATH_SECTION "../RouteDatabase/"
#define SECTION "Section "


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
}


void AdminModeWindow::on_insertRouteButton_clicked()
{
    cout << "Insert Route clicked!" << endl;
}


void AdminModeWindow::on_deleteRouteButton_clicked()
{
    cout << "Delete Route clicked!" << endl;
}

void AdminModeWindow::on_addRecognizedFaceButton_clicked()
{
    cout << "Add New Subject clicked!" << endl;
}

void AdminModeWindow::loadFromDatabase()
{
    for(char aux_section = 'A'; aux_section <= 'C'; aux_section++)
    {
        for(const auto & entry_file : fs::directory_iterator(string(ROUTE_DATABASE_PATH_SECTION) + string(SECTION) + aux_section))
        {   
            QTableWidgetItem* section = new QTableWidgetItem(QString::fromStdString(string(SECTION) + aux_section));
            section->setTextAlignment(Qt::AlignCenter);

            if (!fs::is_directory(entry_file.path()))
            {
                string aux_route_name = entry_file.path().filename().string();
                aux_route_name.erase(aux_route_name.find(".txt"),4);
                
                int tableRow = ui->routeTableWidget->rowCount();
                ui->routeTableWidget->insertRow(tableRow);

                QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(aux_route_name));
                item->setTextAlignment(Qt::AlignCenter);
                ui->routeTableWidget->setItem(tableRow, 0, item);
                ui->routeTableWidget->setItem(tableRow, 1, section);
            } 
        }  
    }

    for(const string& route : RouteRegistration::get_route_names())
    {
        QTableWidgetItem* section = new QTableWidgetItem(QString::fromStdString(" - "));
        section->setTextAlignment(Qt::AlignCenter);

        int tableRow = ui->routeTableWidget->rowCount();
        ui->routeTableWidget->insertRow(tableRow);

        QTableWidgetItem* item = new QTableWidgetItem(QString::fromStdString(route));
        item->setTextAlignment(Qt::AlignCenter);
        ui->routeTableWidget->setItem(tableRow, 0, item);
        ui->routeTableWidget->setItem(tableRow, 1, section);
    }
}
