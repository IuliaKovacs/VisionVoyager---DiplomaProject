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

    camera_widget = new CameraWidget(this);
    ui->layout_2->addWidget(camera_widget);

    file_drop_widget = new FileDropWidget(this);
    ui->dragNdropLayout->addWidget(file_drop_widget);

}

AdminModeWindow::~AdminModeWindow()
{
    delete ui;
}


void AdminModeWindow::on_routeRegistrationButton_clicked()
{
    cout << "routeRegistrationButton clicked!" << endl;
}



