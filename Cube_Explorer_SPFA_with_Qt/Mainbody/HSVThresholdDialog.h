#pragma once

#include <QDialog>
#include <qmap.h>
#include "ui_HSVThresholdDialog.h"
#include "cubeRecognizer.h"


class HSVThresholdDialog : public QDialog
{
	Q_OBJECT

public:
	HSVThresholdDialog(QWidget *parent = Q_NULLPTR);
	~HSVThresholdDialog();

	void showHSVData(QString face);

private:
	Ui::HSVThresholdDialog ui;
	
	QMap<QString, QMap<QString, HSV>> map_hsv_t;	//临时hsv数据,存放被更改但是尚未应用的hsv数值,map.[面].[颜色]进行访问

	QChar curFace = 'f';							//当前显示面

public slots:
	void on_btnResetClicked();						//对话框按钮槽函数
	void on_btnApplyClicked();						//
	void on_btnCancelClicked();						//

	void on_faceRadioToggled(bool checked);			//页面选择按钮槽函数
	void slot_spinValueChanged(int value);			//数据框槽函数
};

