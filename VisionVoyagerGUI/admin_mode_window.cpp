#include "admin_mode_window.h"
#include "ui_adminmodewindow.h"
#include "camera_widget.h"


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

    cameraWidget = new CameraWidget(this);
    ui->layout_2->addWidget(cameraWidget);
}

AdminModeWindow::~AdminModeWindow()
{
    delete ui;
}
