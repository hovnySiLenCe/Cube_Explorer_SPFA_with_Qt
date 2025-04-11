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
#include "ui_DebugWidget.h"
#include "CubeExplorerWithQt.h"
#include "LastSampleDialog.h"
#include "qstring.h"

class DebugWidget : public QDialog
{
	Q_OBJECT

public:
	DebugWidget(QSerialPort* serialPort, QWidget *parent = Q_NULLPTR);
	~DebugWidget();

	const string MechanicalMoveStr[10] = { "M_L1", "M_L2", "M_L3", "M_LC", "M_LO", "M_R1", "M_R2", "M_R3", "M_RC", "M_RO" };
	int Move[120];
	int output[120];
	string steps = "";
	int st;

public slots:
	//查看上次识别采样数据槽函数
	void on_btnShowLastSampleClicked();

	//左爪开合
	void ClickBtnLeftClose();
	void ClickBtnLeftCloseSmall();
	void ClickBtnLeftHalfClose();
	void ClickBtnLeftOpen();
	void ClickBtnLeftOpenSmall();
	void ClickBtnLeftOpenAll();

	//左爪旋转
	void ClickBtnLeftTo90();
	void ClickBtnLeftTo180();
	void ClickBtnLeftClock();
	void ClickBtnLeftAnticlock();
	void ClickBtnLeftClockSmall();
	void ClickBtnLeftAnticlockSmall();
	void ClickBtnLeft180();
	void ClickBtnLeft5Circle();

	//右爪开合
	void ClickBtnRightClose();
	void ClickBtnRightCloseSmall();
	void ClickBtnRightHalfClose();
	void ClickBtnRightOpen();
	void ClickBtnRightOpenSmall();
	void ClickBtnRightOpenAll();

	//右爪旋转
	void ClickBtnRightTo90();
	void ClickBtnRightTo180();
	void ClickBtnRightClock();
	void ClickBtnRightAnticlock();
	void ClickBtnRightClockSmall();
	void ClickBtnRightAnticlockSmall();
	void ClickBtnRight180();
	void ClickBtnRight5Circle();

	//夹子开合
	void ClickBtnClampClose();
	void ClickBtnClampOpen();

	//测试
	void ClickBtntest1();
	void ClickBtntest2();

	//随机序列处理
	void ClickBtnCreate();
	void ClickBtnSave();
	void ClickBtnOpen();
	void ClickBtnRun();

private:
	Ui::DebugWidget ui;
	QSerialPort* serialPortDebug;
	int leftHandTurn, rightHandTurn;
};
