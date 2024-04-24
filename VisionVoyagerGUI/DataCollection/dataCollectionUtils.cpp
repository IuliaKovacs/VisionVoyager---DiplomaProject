#include <OpenXLSX.hpp>
#include <iostream>
#include "../adminModeWindow.h"

using namespace OpenXLSX;

int AdminModeWindow::generateXLS() 
{
    XLDocument doc;
    doc.create("../VisionVoyagerGUI/DataCollection/Spreadsheet.xlsx");
    auto wks = doc.workbook().worksheet("Sheet1");

    cout << "Generez Excel!" << endl;

    wks.cell("A1").value() = "Hello, OpenXLSX!";

    doc.save();

    return 0;
}