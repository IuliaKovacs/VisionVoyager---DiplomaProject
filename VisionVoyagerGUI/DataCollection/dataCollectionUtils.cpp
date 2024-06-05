#include <OpenXLSX.hpp>
#include "../adminModeWindow.h"
#include "../ui_adminmodewindow.h"

#define ROUTE_DATA_EXCEL_PATH "../VisionVoyagerGUI/DataCollection/RoutesData.xlsx"
#define SHEET_ROUTE_NAME "RoutesData"


using namespace OpenXLSX;

int AdminModeWindow::generateXLS() 
{
    XLDocument doc;
    doc.create(ROUTE_DATA_EXCEL_PATH);
    auto wks = doc.workbook().worksheet(SHEET_ROUTE_NAME);

    wks.cell("A1").value() = "Hello, OpenXLSX!";

    doc.save();

    return 0;
}


void AdminModeWindow::initialize_chart()
{
    XLDocument doc;
    doc.open(ROUTE_DATA_EXCEL_PATH);

    auto wks = doc.workbook().worksheet(SHEET_ROUTE_NAME);

    // logFile << log_time() << wks.rowCount() << endl;

    QBarSeries *series = new QBarSeries;

    for(int i = 2; i <= wks.rowCount(); i++)
    {   
        string route_name = wks.cell(i, 1).value().get<std::string>();

        if (route_name.empty()) 
        {
            break;
        }

        int selection_counter = wks.cell(i, 3).value().get<int>();

        QBarSet *set = new QBarSet(QString::fromStdString(route_name));
        *set << selection_counter;
        series->append(set);
    }


    doc.close();

    chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Route Selection Chart");

    QStringList categories {"Total No of Selections per Route"};
    auto axisX = new QBarCategoryAxis;
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    auto axisY = new QValueAxis;
    axisY->setRange(0,15);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}


void AdminModeWindow::delete_excel_row(int row)
{
    XLDocument doc;
    doc.open(ROUTE_DATA_EXCEL_PATH);
    auto wks = doc.workbook().worksheet(SHEET_ROUTE_NAME);
    doc.workbook().addWorksheet("TempSheet");
    auto newWks = doc.workbook().worksheet("TempSheet");

    // logFile << log_time() << "Row = " << row << endl;

    for(int i = row; i < wks.rowCount(); i++)
    {
        for(int j = 1; j <= wks.columnCount(); j++)
        {
           wks.cell(i, j).value() = wks.cell(i+1, j).value(); 
        }
    }

    for(int i = 1; i < wks.rowCount(); i++)
    {
        for(int j = 1; j <= wks.columnCount(); j++)
        {
           newWks.cell(i, j).value() = wks.cell(i, j).value(); 
        }
    }
    
    doc.workbook().deleteSheet(SHEET_ROUTE_NAME);
    newWks.setName(SHEET_ROUTE_NAME);

    // logFile << log_time() << "New count row: " << newWks.rowCount() << endl;

    doc.save();
    doc.close();

    update_chart();
}

void AdminModeWindow::update_chart()
{
    XLDocument doc;
    doc.open(ROUTE_DATA_EXCEL_PATH);
    auto wks = doc.workbook().worksheet(SHEET_ROUTE_NAME);

    QBarSeries *series = new QBarSeries;

    for(int i = 2; i <= wks.rowCount(); i++)
    {   
        string route_name = wks.cell(i, 1).value().get<std::string>();

        if (route_name.empty()) 
        {
            break;
        }

        int selection_counter = wks.cell(i, 3).value().get<int>();

        QBarSet *set = new QBarSet(QString::fromStdString(route_name));
        *set << selection_counter;
        series->append(set);
    }

    chart->removeAllSeries();
    chart->addSeries(series);

    // logFile << log_time() << "Row count update: " << wks.rowCount() << endl;

    doc.save();
    doc.close();
}

void AdminModeWindow::add_route_to_excel(string route_name, string section, int index)
{
    index = index + 2;
    XLDocument doc;
    doc.open(ROUTE_DATA_EXCEL_PATH);
    auto wks = doc.workbook().worksheet(SHEET_ROUTE_NAME);

    // logFile << log_time() << "Index = " << index << endl;

    for(int i = wks.rowCount() + 1; i > index ; i--)
    {
        for(int j = 1; j <= wks.columnCount(); j++)
        {
           wks.cell(i, j).value() = wks.cell(i-1, j).value(); 
        }
    }

    wks.cell(index, 1).value() = route_name;   
    wks.cell(index, 2).value() = section;   
    wks.cell(index, 3).value() = 0;   

    doc.save();
    doc.close();

    update_chart();
}


void AdminModeWindow::populate_logs_list()
{
    QStandardItemModel *model = new QStandardItemModel(this);

    QString folder_path = LOGS_PATH;
    QDir folder(folder_path);

    QStringList log_files = folder.entryList(QDir::Files);

    Q_FOREACH(const QString &file_name, log_files) 
    {
        QStandardItem *item = new QStandardItem(file_name);
        item->setEditable(false); 
        model->appendRow(item);
    }

    ui->logsListView->setModel(model);
}

