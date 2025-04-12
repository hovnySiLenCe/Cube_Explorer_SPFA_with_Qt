#pragma once
#include <QtWidgets/QWidget>
#include <iostream>
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qfile.h>
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

#define MAX_PARAM_NUM 30
struct ParameterSet {
	int param[MAX_PARAM_NUM];
	void SetParameter(int op) {
		int id = op / 10000;
		if (id < 0 || id >= MAX_PARAM_NUM) return;
		param[id] = op % 10000;
	}
};
struct ParaKeySet {
	QString key[MAX_PARAM_NUM];
	QVariantMap toVariantMap(const ParameterSet m_set) {
		QVariantMap map;
		for (size_t i = 0; i < MAX_PARAM_NUM; i++)
		{
			if(!key[i].isEmpty())
				map[key[i]] = QVariant::fromValue(m_set.param[i]);
		}
		return map;
	}
	ParameterSet fromVariantMap(const QVariantMap map) {
		ParameterSet m_set;
		for (size_t i = 0; i < MAX_PARAM_NUM; i++)
		{
			m_set.param[i] = map[key[i]].toInt();
		}
		return m_set;
	}

};

class DataSheetWidget : public QDialog
{
	Q_OBJECT

public:
	DataSheetWidget(QSerialPort* serialPort, QWidget *parent = Q_NULLPTR);
	~DataSheetWidget();

public slots:
	//下位机操作
	void on_btnReadParaClicked();
	void on_btnConfModiClicked();

	// 策略组操作
	void on_btnSaveStrategyClicked();
	void on_btnDelStrategyClicked();
	void on_btnResetStrategyClicked();
	void on_ListItemChanged(QListWidgetItem* item);
	void slot_OpenSelectedStrategy(QListWidgetItem* item);

	// 串口读取
	void slot_ReadParaFromSerial();

private:
	Ui::DataSheetWidget* ui;
	QSerialPort* serialPort;
	QMap<QString, ParameterSet> m_versions;
	ParameterSet m_currentSet;
	ParaKeySet m_keySet;

	// ui同步
	void LoadKeyFromUI();
	void UpdateCurrentSetFromUI();
	void UpdateUIFromCurrentSet();
	bool IsCurrentSetDiffFromUI();

	// 用户行为
	bool IsUserCancelAction(const QString message);

	// 文件管理函数
	bool SaveToFile(const QString& filename);
	void LoadFromFile(const QString& filename);

	// 头文件声明
protected:
	void closeEvent(QCloseEvent* event) override;
};