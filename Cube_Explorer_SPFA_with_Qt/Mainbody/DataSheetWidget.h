#pragma once
#include <QtWidgets/QWidget>
#include <iostream>
#include "CubeRecognizer.h"
#include "CubeExplorer.h"
#include <qserialport.h>
#include <QStringListModel>
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qfiledialog.h>
#include <string>
#include "ui_CubicExplorerWithQt.h"
#include "ui_DataSheetWidget.h"
#include "CubeExplorerWithQt.h"
#include "LastSampleDialog.h"
#include "qstring.h"

class DataSheetWidget : public QDialog
{
	Q_OBJECT

public:
	DataSheetWidget(QSerialPort* serialPort, QWidget *parent = Q_NULLPTR);
	~DataSheetWidget();

public slots:
	//查看上次识别采样数据槽函数

private:
	Ui::DataSheetWidget ui;
	QSerialPort* serialPortDataSheet;
};
