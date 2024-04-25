#include "../CameraModule/person.h"
#include "../CameraModule/cameraModule.h"
#include "adminModeWindow.h"
#include "ui_adminmodewindow.h"
#include "../RouteRegistration/routeRegistrationUtils.h"


void AdminModeWindow::load_from_database()
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

        update_subject_display(tableRow+1, person.get_first_name(), person.get_last_name()[0]);
    }
}


void AdminModeWindow::update_subject_display(int index, string first_name, char letter)
{
    string aux = first_name + " " + letter + ".";
    QString display_text = QString::fromStdString(aux);
    string face_path = string(FACES_DATASET_PATH) + "/s" + to_string(index);
    string filename;

    if (fs::exists(face_path) && fs::is_directory(face_path)) {
        // Iterează prin intrările din director
        for (const auto& entry : fs::directory_iterator(face_path)) 
        {
            filename = entry.path().string();
            break;
        }
    }

    QPixmap pixmap(filename.c_str());
    QSize size(111, 161); 
    QPixmap scaledPixmap = pixmap.scaled(size, Qt::KeepAspectRatio);

    switch(index)
    {   
        case 1:
            ui->subject1Label->setText(display_text);
            ui->img1Label->setPixmap(scaledPixmap);
            break;
        case 2:
            ui->subject2Label->setText(display_text);
            ui->img2Label->setPixmap(scaledPixmap);
            break;
        case 3:
            ui->subject3Label->setText(display_text);
            ui->img3Label->setPixmap(scaledPixmap);
            break;
        case 4:
            ui->subject4Label->setText(display_text);
            ui->img4Label->setPixmap(scaledPixmap);
            break;
        default:
            /* Do nothing - this should be unreachable */
            cout << "Error" << endl; 
    }
}


void AdminModeWindow::delete_subject_display()
{
    ui->subject1Label->setText("Empty S1");
    ui->img1Label->clear();

    ui->subject2Label->setText("Empty S2");
    ui->img2Label->clear();

    ui->subject3Label->setText("Empty S3");
    ui->img3Label->clear();

    ui->subject4Label->setText("Empty S4");
    ui->img4Label->clear();
}