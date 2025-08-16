#pragma once

#include <QMessageBox>
#include "CubeRecognizer.h"
#include "HSVDataDialog.h"
#include <QDialog>
#include <qpixmap.h>
#include "ui_HSVCorrectionDialog.h"
#include "qdebug.h"
#include <qjsondocument.h>
#include <qjsonarray.h>
#include <qjsonobject.h>
#include <qfile.h>
#include <qfiledialog.h>

class HSVCorrectionDialog : public QDialog
{
	Q_OBJECT

public:
	HSVCorrectionDialog(QWidget *parent = Q_NULLPTR);
	~HSVCorrectionDialog();

	void clearLayout(QGridLayout* layout);

	// 按钮显示结果值栏新增控件
	void addButtonToResultGrid(QString strFace);
	//设置采样按钮图标为当前面的各个采样图
	void setIconBySample(QPushButton* btn, QString strFace, int id); 

	// 在实际值栏新增控件操作
	void addLineEditToRealGrid(QString strFace);
	void addLineEditsToLayout(QGridLayout* layout, const QString& face, const QString& hsvType);
	void addLineEditToCorrectionGrid(QString strFace);
	// 实际值栏高亮
	void displayStandardHighlight(); // 将基准值的lineEdit高亮显示
	// 实际值栏值计算
	void calcRealHSV(int H_theroshold = 150);
	void displayRealHSV();

	// 在单个Layout布局中更新或读取参数
	void readCorrectionFromLayout(QGridLayout* layout, int beginId, vector<int>& correction); // 将界面上的数据读入向量中
	void displayCorrectionOnLayout(QGridLayout* layout, int beginId, vector<int>& correction); // 显示向量中的数据

	// 文件存储操作
	void LoadCorreDataFromFile(const QString& filename);
	bool SaveCorreDataToFile(const QString& filename);

	// 校正值参数UI操作
	bool IsCurrentSetDiffFromUI(); // 当前参数集是否与UI不同
	bool IsUserCancelAction(const QString message); // 用户是否取消操作
	void UpdateCurrentSetFromUI(); // 从UI更新当前参数集
	void UpdateUIFromCurrentSet(); // 从当前参数集更新UI

private:
	Ui::HSVCorrectionDialog* ui;
	QList<QString> m_listFace = { "u", "r", "f", "d", "l", "b" };
	QList<QString> m_listHSV = { "H", "S", "V" };
	QMap<QString, QMap<QString, vector<int> > > m_correDataset;
	QMap<QString, vector<int>>& m_correData;
	QMap<QString, vector<int>> m_curData;
	QMap<QString, vector<int>> m_realData;
	QMap<QString, vector<cv::Mat>> mat_map; // 保存Recognizer中的识别区域色块，key为面
	//vector<int>& correction_H, correction_S, correction_V;
	pair<QString, int> standardIndex;

public slots:
	void onbtnSampleBlockClicked(); // 当采样块按下后显示详细信息

	void onbtnReadParaClicked();// 当按下读取参数按钮后读取参数
	void onbtnAutoCorrectClicked(); // 当按下自动校正按钮后开始自动校正

	void onbtnConfModiClicked(); // 当确认修改按钮按下后保存修改

	// 真实值栏刷新按钮
	void onbtnRefreshDisplayClicked();

	// 校正参数操作
	void onbtnSaveVersionClicked(); // 当按下保存版本按钮后保存当前参数
	void onbtnDelVersionClicked(); // 当按下删除版本按钮后删除当前版本
	void onListItemChanged(QListWidgetItem* item); // 当列表项改变时，对该项进行重命名
	void slot_OpenSelectedVersion(QListWidgetItem* item); // 双击打开选项
	void slot_RefreshVersionName(QListWidgetItem* item); // 单击列表项时在列表框刷新名称

	// 校正参数版本文件操作
	void onbtnExportFileClicked();
	void onbtnImportFileClicked();

	// 在关闭时将参数保存在历史参数
protected:
	void closeEvent(QCloseEvent* event) override;
};
