#pragma once

#include "CubeRecognizer.h"
#include "HSVDataDialog.h"
#include <QDialog>
#include <qpixmap.h>
#include "ui_LastSampleDialog.h"

class LastSampleDialog : public QDialog
{
	Q_OBJECT

public:
	LastSampleDialog(QWidget *parent = Q_NULLPTR);
	~LastSampleDialog();

	void setIconBySample();					//设置采样按钮图标为当前面的各个采样图

private:
	Ui::LastSampleDialog ui;

	QString curFace;
	QList<QPushButton*> list_btnBlock;		//根据序号得到对应九个采样图的按钮

public slots:
	void on_btnSampleBlockClicked();
	void on_radioFaceToggled(bool checked);
};
