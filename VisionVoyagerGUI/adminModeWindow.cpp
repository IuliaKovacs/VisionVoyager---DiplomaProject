#include "adminModeWindow.h"
#include "ui_adminmodewindow.h"
#include "cameraWidget.h"
#include "../CameraModule/cameraModule.h"
#include "../CameraModule/person.h"
#include "../ApplicationModule/application_utils.h"
#include <filesystem>
#include <cstdio>
#include <iterator> 

namespace fs = std::filesystem;
using namespace std; 

#define ROUTE_DATABASE_PATH_SECTION "../RouteDatabase/"
#define SECTION "Section "
#define FILE_TERMINATION ".txt"
#define MAX_NO_OF_SUBJECTS 4
#define MINIMUM_IMG_NO_TO_ADD 7

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
    ui->personsTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->personsTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->errorLabel->setVisible(false);
    ui->addErrorLabel->setVisible(false);

    load_from_database();
    load_recognized_persons();
    initialize_chart();
    populate_logs_list();
    languge_buttons_set();

    camera_widget = new CameraWidget(this);
    ui->layout_2->addWidget(camera_widget);

    file_drop_widget_fr = new FileDropWidget(this);
    ui->dragNdropLayout->addWidget(file_drop_widget_fr);

    file_drop_route = new FileDropWidget(this);
    file_drop_route->setAcceptsSingleFile(true);
    ui->routeDragNdropLayout->addWidget(file_drop_route);
    
    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    ui->chartLayout->addWidget(chartView);
    
}

AdminModeWindow::~AdminModeWindow()
{
    delete ui;
}

void AdminModeWindow::on_startRegistrationButton_clicked()
{
    logFile << log_time() << "Start register clicked!" << endl;
    RouteRegistration::set_register_enabled_true();
}


void AdminModeWindow::on_endReigstrationButton_clicked()
{
    logFile << log_time() << "Stop register clicked!" << endl;
    RouteRegistration::end_registration();
}


void AdminModeWindow::on_saveChangesButton_clicked()
{
    logFile << log_time() << "Save Changes clicked!" << endl;
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
                // logFile << log_time() << old_file_name << endl;
                // logFile << log_time() << new_file_name << endl;
                rename(old_file_name.c_str(), new_file_name.c_str());
            }
        }
    }
}


void AdminModeWindow::on_insertRouteButton_clicked()
{
    logFile << log_time() << "Insert Route clicked!" << endl;
    string new_route_name = (ui->routeNameLineEdit->text()).toStdString();
    string new_route_section = (ui->comboBox->currentText()).toStdString();
    
    if(!new_route_name.empty())
    {
        if(((new_route_section == "Section A") && (section_A_routes >= 3))
        || ((new_route_section == "Section B") && (section_B_routes >= 3))
        || ((new_route_section == "Section C") && (section_C_routes >= 3)))
        {
            ui->errorLabel->setVisible(true);
            ui->errorLabel->setText("Error: The selected section is full! Please delete an entry first!");
        }
        else 
        {
            ui->errorLabel->setVisible(false);
            if(file_drop_route->getListCount() == 1)
            {   
                QListWidgetItem item = file_drop_route->getFirstItem();
                string path = (item.text()).toStdString();
                string filename = get_filename_from_path(path);
                bool ok = true;

                for(const string path : RouteRegistration::get_route_paths())
                {
                    string aux_filename = get_filename_from_path(path);
                    if(aux_filename == new_route_name)
                    {
                        ui->errorLabel->setVisible(true);
                        ui->errorLabel->setText("Error: The route name already exists! Please choose another one!");
                        ok = false;
                        break;
                    }
                }

                if(ok == true)
                {   
                    string filename_path = string(ROUTE_DATABASE_PATH_SECTION) + new_route_section + "/" + new_route_name;
                    string destination = string(ROUTE_DATABASE_PATH_SECTION) + new_route_section + "/" + new_route_name + string(FILE_TERMINATION);

                    try 
                    {
                        fs::copy_file(path, destination);
                        // logFile << log_time() << "File copied successfully!" << endl;
                        vector<string>& route_paths = RouteRegistration::get_route_paths();
                        route_paths.push_back(filename_path);
                        sort(route_paths.begin(), route_paths.end());
                        auto it = std::find(route_paths.begin(), route_paths.end(), filename_path);
                        int index = std::distance(route_paths.begin(), it);
                        add_route_to_excel(new_route_name, new_route_section, index);
                        load_from_database();
                        file_drop_route->clearFileList();
                        ui->routeNameLineEdit->clear();
                    } 
                    catch(const fs::filesystem_error& e) 
                    {
                        logFile << log_time() << "Error: " << e.what() << endl;
                    }
                }
            }
            else
            {
                ui->errorLabel->setVisible(true);
                ui->errorLabel->setText("Error: No route file added! Please add a file first!");
            }
        }
    }
    else
    {
        ui->errorLabel->setVisible(true);
        ui->errorLabel->setText("Error: The route name field is empty! Please add a name");
    }

}


void AdminModeWindow::on_deleteRouteButton_clicked()
{
    logFile << log_time() << "Delete Route clicked!" << endl;
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
                // logFile << log_time() << file_path << endl;
                vector<string>& route_paths = RouteRegistration::get_route_paths();
                route_paths.erase(route_paths.begin() + row);
                remove(file_path.c_str());
                if(section == "Section A")
                {
                    section_A_routes--; 
                }
                else if(section == "Section B")
                {
                    section_B_routes--; 
                }
                else if(section == "Section C")
                {
                    section_C_routes--; 
                }
                /* @Todo - check if is needed to also update the tts files */
            }
            else
            {
                string file_path = string(ROUTE_DATABASE_PATH_SECTION) + route_name + string(FILE_TERMINATION);
                // logFile << log_time() << file_path << endl;
                remove(file_path.c_str());
            }

            ui->routeTableWidget->removeRow(row);
            delete_excel_row(row + 2);
        }
        else
        {
            logFile << log_time() << "Error: There is a problem with deleting the route!" << endl;
        }
    }


}

void AdminModeWindow::on_addRecognizedFaceButton_clicked()
{
    logFile << log_time() << "Add New Subject clicked!" << endl;
    string first_name = (ui->firstNameLineEdit->text()).toStdString();
    string last_name = (ui->lastNameLineEdit->text()).toStdString();
    string role = (ui->roleComboBox->currentText()).toStdString();

    if(!first_name.empty())
    {
        if(!last_name.empty())
        {
            if(MAX_NO_OF_SUBJECTS > CameraModule::count_recognized_subjects())
            {   
                if(file_drop_widget_fr->getListCount() >= MINIMUM_IMG_NO_TO_ADD)
                {
                    ui->addErrorLabel->setVisible(false);
                   
                    QListWidget* fileList = file_drop_widget_fr->getFileListWidget();

                    int new_index = CameraModule::count_recognized_subjects() + 1;
                    string new_path = string(RAW_IMAGES_FOLDER_PATH);

                    for (int i = 0; i < fileList->count(); i++)
                    {   
                        QListWidgetItem *item = fileList->item(i);
                        string path = (item->text()).toStdString();
                        string filename = get_filename_from_path(path);
                        string destination = new_path + "/" + filename;
                        // logFile << log_time() << destination << endl;
                        fs::copy_file(path, destination);
                    }
                    CameraModule::add_new_recognized_subject();
                    string id = "s" + to_string(new_index);
                    Person p = Person(id, first_name, last_name, role);
                    vector<Person>& persons = CameraModule::get_recognized_persons();
                    persons.push_back(p);
                    
                    CameraModule::write_recognized_persons();
                    
                    load_recognized_persons();

                    file_drop_widget_fr->clearFileList();
                    ui->firstNameLineEdit->clear();
                    ui->lastNameLineEdit->clear();

                    CameraModule::create_csv_database_file();
                }
                else
                {
                    ui->addErrorLabel->setVisible(true);
                    ui->addErrorLabel->setText("Error: Please add more images!");
                }
            }
            else
            {
                ui->addErrorLabel->setVisible(true);
                ui->addErrorLabel->setText("Error: The list is full! Delete an entry first!");
            }
        }
        else
        {
            ui->addErrorLabel->setVisible(true);
            ui->addErrorLabel->setText("Error: Last Name field is empty!");
        }
    }
    else
    {
        ui->addErrorLabel->setVisible(true);
        ui->addErrorLabel->setText("Error: First Name field is empty!");
    }
}


void AdminModeWindow::on_deletePersonButton_clicked()
{
    logFile << log_time() << "Delete Person clicked!" << endl;

    QList<QTableWidgetItem*> selectedItems = ui->personsTableWidget->selectedItems();

    for (QTableWidgetItem* item : selectedItems) 
    {
        int row = item->row();
        
        vector<Person>& persons = CameraModule::get_recognized_persons();
        persons.erase(persons.begin() + row);
        
        CameraModule::write_recognized_persons();

        string subject_folder_path = string(FACES_DATASET_PATH) + "/s" + to_string(row + 1); 
        try 
        {
            fs::remove_all(subject_folder_path);
            logFile << log_time() << "Erased the " << subject_folder_path << " folder successfully!" << endl;
        }
        catch (const fs::filesystem_error& e) 
        {
            logFile << log_time() << "Error: There was a error in erasing the subject folder: " << e.what() << endl;
        }

        CameraModule::update_faces_dataset_namings();

        CameraModule::create_csv_database_file();

        ui->personsTableWidget->removeRow(row);
    }

    delete_subject_display();
    load_recognized_persons();
}


void AdminModeWindow::on_logsListView_clicked(const QModelIndex &index)
{
    QString log_text = index.data(Qt::DisplayRole).toString();
    QString filename = LOGS_PATH + log_text;
    QProcess::startDetached("xdg-open", QStringList() << filename);
}

void AdminModeWindow::on_controlButton_clicked(bool checked)
{
    if (checked)
    {
        KeyboardControl::set_keyboard_control_active(true);
    }
    else
    {
        KeyboardControl::set_keyboard_control_active(false);
    }
}


void AdminModeWindow::on_enButton_clicked(bool checked)
{
    if(checked)
    {
        set_language_EN();
        ui->roButton->setChecked(false);
    }
}


void AdminModeWindow::on_roButton_clicked(bool checked)
{
    if(checked)
    {
        set_language_RO();
        ui->enButton->setChecked(false);
    }
}