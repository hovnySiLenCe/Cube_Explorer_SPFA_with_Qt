#pragma once
	
#include <vector>
#include <QDialog>
#include <qlabel.h>
#include <qpixmap.h>
#include <qgroupbox.h>
#include <qradiobutton.h>
#include <QtDataVisualization/Q3DSurface>
#include <QtDataVisualization/QSurfaceDataProxy>
#include <QtDataVisualization/QHeightMapSurfaceDataProxy>
#include <QtDataVisualization/QSurface3DSeries>
#include <qsurface.h>
#include "CubeRecognizer.h"
#include "ui_HSVDataDialog.h"

using namespace QtDataVisualization;

#define SAMPLED_PIC_SHOW_WIDTH 150			//采样图片显示在窗口上的宽度（高度等比例变化）

class HSVDataDialog : public QDialog
{
	Q_OBJECT

public:
	HSVDataDialog(cv::Mat imageBGR,QWidget *parent = Q_NULLPTR);
	~HSVDataDialog();

	void calculation(string target);		//根据m_matHSV得到HSV数据并计算极大、极小、均值
	void iniGraph();						//初始化图表UI
	void iniStatistic();					//初始化采样数据统计UI
	
private:
	cv::Mat m_matHSV;
	cv::Mat m_matRGB;

	//数据可视化
	Q3DSurface *surface_graph;
	QWidget *containner;
	QSurfaceDataProxy *proxy; 
	QSurface3DSeries *series;
	QSurfaceDataArray *dataArray_H;
	QSurfaceDataArray *dataArray_S;
	QSurfaceDataArray *dataArray_V;

	//采样数据展示与数据选择
	QVBoxLayout *vLay_imgAndChannel;

	QGroupBox *groupBox_sample;				//采样数据显示
	QVBoxLayout *vLay_samData;				//
	QLabel *label_showSamPic;				//
	QImage *m_imgRGB;						//
	QLabel *label_max;						//
	QLabel *label_min;						//
	QLabel *label_mean;						//
	int nMax_H;								//极大、极小、均值
	int nMin_H;								//
	int nMean_H;							//
	int nMax_S;								//
	int nMin_S;								//
	int nMean_S;							//
	int nMax_V;								//
	int nMin_V;								//
	int nMean_V;							//

	QGroupBox *groupBox_sourceSelect;		//数据选择
	QVBoxLayout *vLay_radioBtn;				//
	QRadioButton *radioBtn_H;				//
	QRadioButton *radioBtn_S;				//
	QRadioButton *radioBtn_V;				//

	Ui::HSVDataDialog ui;

public slots:
	void on_radioBtnHToggled(bool checked);
	void on_radioBtnSToggled(bool checked);
	void on_radioBtnVToggled(bool checked);
};
