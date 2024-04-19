#ifndef ADMIN_MODE_WINDOW_H
#define ADMIN_MODE_WINDOW_H


#include <QApplication>
#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "cameraWidget.h"
#include "fileDropWidget.h"


void start_GUI(int argc, char *argv[]);


QT_BEGIN_NAMESPACE
namespace Ui 
{
    class AdminModeWindow;
}
QT_END_NAMESPACE

class AdminModeWindow : public QMainWindow
{
    Q_OBJECT

public:
    AdminModeWindow(QWidget *parent = nullptr);
    ~AdminModeWindow();

private slots:
    void on_routeRegistrationButton_clicked();

private:
    Ui::AdminModeWindow *ui;
    CameraWidget *camera_widget; 
    FileDropWidget *file_drop_widget_fr;
    FileDropWidget *file_drop_route;
};


#endif // ADMIN_MODE_WINDOW_H
