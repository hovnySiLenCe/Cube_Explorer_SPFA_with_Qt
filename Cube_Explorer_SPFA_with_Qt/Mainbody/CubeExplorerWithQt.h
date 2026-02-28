#pragma once
#include <QtWidgets/QMainWindow>
#include <QInputDialog>
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
#include "HSVCorrectionDialog.h"

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
	CubeExplorerWithQt(QWidget* parent = Q_NULLPTR);
	virtual ~CubeExplorerWithQt();
	//void openCamera();
	
	void InitCameraEvents();
	void CaptureImage();
	void ShowRecogResultOnScene(string strRec);
	void Sleep(int);
	void SolveAndRestore();

	cv::Mat QImageToCvMat(const QImage& image);						// 将QImage转化为CvMat
	void SaveCaptureMatToFile(string curPath);

private:
	Ui::CubicExplorerWithQt ui;

	int st, ed;
	string recogResult; // 识别结果
	char* kociembaResult; 
	//string ansOpSequence; // Solve2序列

	bool isCameraOpen = false;
	bool isToRestore = false;
	bool hasRobotStarted = false;
	bool inputFromBox = false;
	const int captureInterval = 1000;

	CubeExplorerSPFA* cubeExplorerSPFA;
	MultiSolver* multiSolver;
	
	QTimer* timer_displayRefresh;						// 复原计时触发器
	MyTimer* timer_stopWatch;							// 复原计时器
	MyTimer* timer_cp_stopWatch;						// 竞速模式电脑计时器
	MyTimer* timer_user_stopWatch;						// 竞速模式玩家计时器
	
	QList<RestoreRecord> list_restoreRecords;			// 复原记录

	QTimer* timeoutTimer;
	QTimer* handReleaseDalayTimer;
	QByteArray* byteTmp;
	QByteArray* comByteBuffer;

	QSerialPort* serialPort;							//串口控制
	QSerialPortInfo currentPortInfo;
	QMap<QString, QSerialPortInfo> list_portInfo;

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
	
	
	int nImgSaved;
	cv::Mat captureMatSet[4];
	QList<QGraphicsRectItem*> list_samRecItems;			//存储显示在窗口上的采样框指针QByteArray byteTmp;

	// 打乱序列生成
	const QString actCommandStr[10] = {
		"L1", "L2", "L3", "LC", "LO",
		"R1", "R2", "R3", "RC", "RO" };

	int actSeq[503];
	int seqsLength;
	QString displaySeqs;

	int handAngle[2];

	void InitTimerComponent();
	void SetHighlightButtom(QPushButton* buttom);
	void SetCommonStyButtom(QPushButton* buttom);

	void CleanSolverResultDisplay();
	void SetSolverResultDisplay();

	void LoadRestoreRecordsFromFile();
	void LoadRestoreRecordsFromCSV(QString filename = "./Data/RestoreRecord.csv");
	void SaveRestoreRecordsToFile();
	void SaveRestoreRecordsToCSV(QString filename = "./Data/RestoreRecord.csv");
	void AppendRestoreRecordsToTable(QString str_description);
	QString FormatCSVField(const QVariant& value);

protected:
	void keyPressEvent(QKeyEvent* event) override;

public slots:
	//界面按钮槽函数
	void on_btnTightOrLooseClicked();
	void on_btnRestoreClicked();
	void on_btnSendSingleClicked();
	void on_btnDebugClicked();
	void onbtnCamSwitchClicked();
	void on_btnShowSamRecsClicked();
	void on_btnRecogClicked();
	void on_btnShowSampleResultClicked();
	void onSetDataSheetClicked();
	void slot_btnResetClicked();
	void slot_btnHandOCClicked();

	// 打乱序列生成
	void resetAngle(int handId, bool isTwist);
	void turn(int handId, int turnId);
	void twist(int handId, int turnId);
	void slot_btnRandCreateClicked();
	void slot_btnRandRunClicked();

	// 交互-竞速模式函数
	void slot_btnCompStartClicked();

	//摄像头View鼠标响应槽函数
	void slot_mouseReleasedInCameraViews(QRect rec_select);
	void slot_menuSetRecTriggered();
	void slot_menuShowHSVTriggered();
	//取色块设置响应槽函数
	void slot_setRecArea(QString groupName, QRect rect, int faceID, int blockID);
	//Capture响应槽函数
	void slot_imageCaptured(int id, const QImage& image);
	//串口模块响应槽函数
	void slot_portInfoChanged();
	void on_btnPortRefreshClicked();
	void on_btnPortOpenClicked();
	void on_btnPortSendClicked();
	//摄像头分配槽函数
	void slot_cameraInfoChanged(const QString& text);

	void slot_sendOperationSerial();

	void TimerDisplayRefresh();
	void slot_comReadyRead();
	void ReadOperationFromPort();
	
	void slot_baudRateChanged();
	//void on_btnStrategyConfirm();

	void WaitForPortReadTimeout();
	void slotInputStateChange(); // 输入状态改变功能
	void slotReuseStateChange(); // 时间复用功能

	//void box_speedModified();
};
