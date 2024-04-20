#include "adminModeWindow.h"
#include "ui_adminmodewindow.h"
#include "cameraWidget.h"
#include <iostream>

using namespace std;


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


