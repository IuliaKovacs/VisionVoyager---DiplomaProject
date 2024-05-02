#ifndef ADMIN_MODE_WINDOW_H
#define ADMIN_MODE_WINDOW_H



#include <QApplication>
#include <QMainWindow>
#include <QtCharts/QtCharts>
#include <opencv2/opencv.hpp>
#include "cameraWidget.h"
#include "fileDropWidget.h"
#include "../ApplicationModule/application_utils.h"
#include "../RouteRegistration/routeRegistrationUtils.h"
#include "../KeyboardControl/keyboardControl.h"

using namespace std;

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

protected:
    void keyPressEvent(QKeyEvent *event);


private Q_SLOTS:
    void on_addRecognizedFaceButton_clicked();
    void on_startRegistrationButton_clicked();
    void on_endReigstrationButton_clicked();
    void on_saveChangesButton_clicked();
    void on_insertRouteButton_clicked();
    void on_deleteRouteButton_clicked();
    void on_deletePersonButton_clicked();
    void on_logsListView_clicked(const QModelIndex &index);
    void on_controlButton_clicked(bool checked);


private:
    Ui::AdminModeWindow *ui;
    CameraWidget *camera_widget; 
    FileDropWidget *file_drop_widget_fr;
    FileDropWidget *file_drop_route;
    QChart *chart;
    int section_A_routes = 0;
    int section_B_routes = 0;
    int section_C_routes = 0;

    void load_from_database();
    void initialize_chart();
    void delete_excel_row(int row);
    void add_route_to_excel(string route_name, string section, int index);
    void update_chart();
    void load_recognized_persons();
    void update_subject_display(int index, string first_name, char letter);
    void delete_subject_display();
    string get_filename_from_path(string path);
    int generateXLS(); 
    void populate_logs_list();
};


#endif // ADMIN_MODE_WINDOW_H
