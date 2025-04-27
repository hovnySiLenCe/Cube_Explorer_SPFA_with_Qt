#pragma once
#include <QtWidgets/QMainWindow>
#include <iostream>
#include <qapplication.h>
#include <qdatetime.h>
#include <qserialport.h>
#include <qserialportinfo.h>
#include <QStringListModel>
#include <qfiledialog.h>
#include <qcamera.h>
#include <qcamerainfo.h>
#include <qcameraimagecapture.h>
#include <qtimer.h>
#include <qgraphicsview.h>
#include <qgraphicsscene.h>
#include <qgraphicsitem.h>
#include <qgraphicsvideoitem.h>
#include <QMyGraphicsView.h>

#include <QThread>
#include <QTextCodec>
#include <QVideoProbe>

#include "ui_CubicExplorerWithQt.h"
#include "MyTimer.h"
#include "qstring.h"
#include "qdebug.h"

#include "DebugWidget.h"
#include "DataSheetWidget.h"
#include "RecogAreaDialog.h"
#include "HSVDataDialog.h"
#include "HSVThresholdDialog.h"

#include "CubeRecognizer.h"	
#include "CubeExplorer.h"
#include "CubeExplorerSPFA.h"

#include "MultiSolver.h"

extern "C" {
#include "solve.h"
}

#define SCENE_VIEW_WIDTH 320
#define SCENE_VIEW_HEIGHT 180
struct RestoreRecord {
	int cnt;
	double time;
	RestoreRecord(int n, double t) :cnt(n), time(t) {}
};

enum SCENE_ID {
	SCENE_FR_ID,
	SCENE_U_ID,
	SCENE_BL_ID,
	SCENE_D_ID
};

class CubeExplorerWithQt : public QMainWindow
{
	Q_OBJECT

public:
	virtual ~CubeExplorerWithQt();
	//void openCamera();
	
	void InitCameraEvents();
	void CaptureImage();
	void ShowRecogResultOnScene(string strRec);
	void Sleep(int);
	void SolveAndRestore();

	cv::Mat QImageToCvMat(const QImage& image); // 将QImage转化为CvMat
	void SaveCaptureMatToFile(string curPath);

	void LoadRestoreRecordsFromFile();									//从数据文件读取记录
	void SaveRestoreRecordsToFile();								//将记录写入文件

	CubeExplorerWithQt(QWidget* parent = Q_NULLPTR);

private:
	Ui::CubicExplorerWithQt ui;

	bool isCameraOpen = false;
	bool isToRestore = false;
	bool hasRobotStarted = false;
	bool inputFromBox = false;
	const int captureInterval = 1000;

	//CubeExplorer  cubeExplorer;						   	//魔方解算
	/*CubeExplorer1* cubeExplorer1;	*/					//魔方解算

	CubeExplorerSPFA* cubeExplorerSPFA; // ！！new
	MultiSolver* multiSolver;
	
	QTimer* pTimer;										//复原计时触发器
	MyTimer* pMyTimer;									//复原计时器
	QList<RestoreRecord> list_restoreRecords;			//复原记录

	QTimer* timeoutTimer;
	QTimer* handReleaseDalayTimer;
	QByteArray* byteTmp;
	QByteArray* comByteBuffer;

	QSerialPort* serialPort;							//串口控制
	QSerialPortInfo currentPortInfo;
	QMap<QString, QSerialPortInfo> list_portInfo;

	//摄像头处理
	//QGraphicsScene* scene_FR;							//摄像头显示场景
	//QGraphicsScene* scene_U;							//
	//QGraphicsScene* scene_BL;							//
	//QGraphicsScene* scene_D;							//
	//QGraphicsVideoItem* videoItem_FR;					//摄像头显示载体
	//QGraphicsVideoItem* videoItem_U;					//
	//QGraphicsVideoItem* videoItem_BL;					//
	//QGraphicsVideoItem* videoItem_D;					//
	QRect rec_tSelect;									//选取框位置临时记录
	QString curPath;									//当前工作目录
	
	QList<QCamera*> cameras;							//摄像头列表和摄像头捕获对象列表，两者下标对应
	QList<QCameraImageCapture*> cameraCaptures;			//

	QList<QComboBox*> cameraCombos;						//将所有 QComboBox 指针放入容器
	QList<QGraphicsView*> cameraViews;					//将所有 QGraphicsView 指针放入容器，包含Scene和VideoItem
	QList<QGraphicsScene*> cameraScenes;				//将所有 QGraphicsScene 指针放入容器
	QList<QGraphicsVideoItem*> cameraVideoItems;		//将所有 QGraphicsVideoItem 指针放入容器
	
	QMap<QString, SCENE_ID> sceneNameToIndex;			//图片名到摄像头对象指针的映射
	QMap<QCameraImageCapture*, int> captureToSceneIndex;	

	QList<QCameraInfo> list_cameraInfo;					//摄像头信息
	
	//QList<QVideoProbe*> list_pSnap;
	//QMap<QString, QGraphicsVideoItem*> videoItems;		//图片名到videoItem指针的映射
	//QMap<QString, QGraphicsScene*> map_pic_pScene;		//图片名到scene指针的映射
	
	int nImgSaved;
	cv::Mat captureMatSet[4];

	QList<QGraphicsRectItem*> list_samRecItems;			//存储显示在窗口上的采样框指针QByteArray byteTmp;

public slots:
	//界面按钮槽函数
	void on_btnTightOrLooseClicked();
	void on_btnRestoreClicked();
	void on_btnResetClicked();
	void on_btnSendSingleClicked();
	void on_btnDebugClicked();
	void onbtnCamSwitchClicked();
	void on_btnShowSamRecsClicked();
	void on_btnRecogClicked();
	void on_btnSetHSVClicked();
	void onSetDataSheetClicked();

	//摄像头View鼠标响应槽函数
	void slot_mouseReleasedInCameraViews(QRect rec_select);
	void slot_menuSetRecTriggered();
	void slot_menuShowHSVTriggered();
	//取色块设置响应槽函数
	void slot_setRecArea(QString groupName, QRect rect, int faceID, int blockID);
	//Capture响应槽函数
	void slot_imageCaptured(int id, const QImage& image);
	//串口模块响应槽函数
	void on_btnPortRefreshClicked();
	void on_btnPortOpenClicked();
	void on_btnPortSendClicked();
	//摄像头分配槽函数
	void slot_cameraInfoChanged(const QString& text);

	void slot_sendOperationSerial();

	void slot_timeout();
	void slot_comReadyRead();
	void ReadOperationFromPort();
	
	void slot_baudRateChanged();
	void on_btnStrategyConfirm();

	void slot_onReceiveTimeout();
	void slotInputStateChange(); // 输入状态改变功能
	void slotReuseStateChange(); // 时间复用功能

	//void box_speedModified();
};
