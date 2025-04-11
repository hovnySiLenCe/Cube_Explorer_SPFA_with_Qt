#pragma once

#include <QDialog>
#include <qgridlayout.h>
#include <qmessagebox.h>
#include "CubeExplorerWithQt.h"
#include "ui_RecogAreaDialog.h"

class RecogAreaDialog : public QDialog
{
	Q_OBJECT

public:
	RecogAreaDialog(QString caller,QRect rect,QWidget *parent = Q_NULLPTR);
	~RecogAreaDialog();

private:
	Ui::RecogAreaDialog ui;

	QGridLayout *gridLayout;
	QString m_strCaller;
	QRect m_rec;

signals:
	void signal_setRecArea(QString groupName,QRect rect,int faceID,int blockID);
//	void signal_setRecArea(QString groupName, QRect rect, int faceID, int blockID);

public slots:
	void on_btnsClicked();
};
