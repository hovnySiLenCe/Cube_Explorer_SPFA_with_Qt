#include "DataSheetWidget.h"
#include "ui_DataSheetWidget.h"

DataSheetWidget::DataSheetWidget(QSerialPort* serialPort, QWidget* parent) :
    serialPortDataSheet(serialPort), QDialog(parent), ui(Ui::DataSheetWidget())
{
    ui.setupUi(this);
}

DataSheetWidget::~DataSheetWidget()
{
    delete &ui;
}
