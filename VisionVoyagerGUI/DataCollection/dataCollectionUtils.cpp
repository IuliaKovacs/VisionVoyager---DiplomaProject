#include <OpenXLSX.hpp>
#include <iostream>
#include "../adminModeWindow.h"

#define ROUTE_DATA_EXCEL_PATH "../VisionVoyagerGUI/DataCollection/RoutesData.xlsx"
#define SHEET_ROUTE_NAME "RoutesData"

using namespace OpenXLSX;

int AdminModeWindow::generateXLS() 
{
    XLDocument doc;
    doc.create(ROUTE_DATA_EXCEL_PATH);
    auto wks = doc.workbook().worksheet(SHEET_ROUTE_NAME);

    cout << "Generez Excel!" << endl;

    wks.cell("A1").value() = "Hello, OpenXLSX!";

    doc.save();

    return 0;
}


void AdminModeWindow::initialize_chart()
{
    XLDocument doc;
    doc.open(ROUTE_DATA_EXCEL_PATH);

    auto wks = doc.workbook().worksheet(SHEET_ROUTE_NAME);
    int row_count = wks.rowCount();

    cout << "row_count: " << row_count << endl;

    QBarSeries *series = new QBarSeries;

    for(int i = 2; i <= row_count; i++)
    {   
        string route_name = wks.cell(i, 1).value().get<std::string>();

        if (route_name.empty()) 
        {
            cout << "Sfârșitul datelor la rândul " << i << endl;
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

    QStringList categories {"Total Selections"};
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