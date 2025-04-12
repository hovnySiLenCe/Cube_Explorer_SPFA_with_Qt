#include "CubeExplorerWithQt.h"


extern QList<QString> list_picID;

CubeExplorerWithQt::CubeExplorerWithQt(QWidget *parent)
	: QMainWindow(parent)
{
	//构造CubicExplorer对象
	//cubeExplorer = CubeExplorer();

	cubeExplorerSPFA = new CubeExplorerSPFA();
	cubeExplorerSPFA->InitOrientation();

	multiSolver = new MultiSolver();
	multiSolver->InitSolver();

	ui.setupUi(this);

	curPath = QDir::currentPath();		//获取当前工作路径

	pTimer = new QTimer(this);			//构造并绑定计时器槽函数
	pMyTimer = new MyTimer();
	connect(pTimer, &QTimer::timeout, this, &CubeExplorerWithQt::slot_timeout);

	// 串口延时接收计时器
	byteTmp = new QByteArray(); // 用于接收串口信息
	timeoutTimer = new QTimer(this);
	timeoutTimer->setSingleShot(true);
	connect(timeoutTimer, &QTimer::timeout, this, &CubeExplorerWithQt::slot_onReceiveTimeout);

	// 延时松手计时器
	handReleaseDalayTimer = new QTimer(this);
	handReleaseDalayTimer->setSingleShot(true);
	connect(handReleaseDalayTimer, &QTimer::timeout, this, &CubeExplorerWithQt::on_btnResetClicked);

	/*connect(ui.btn_tightOrLoose, SIGNAL(clicked()), this, SLOT(on_btnTightOrLooseClicked()));*/
	
	// 开始复原按钮
	connect(ui.btn_restore, SIGNAL(clicked()), this, SLOT(on_btnRestoreClicked()));

	// 普通操作按钮
	connect(ui.btn_camSwitch, SIGNAL(clicked()), this, SLOT(on_btnCameraClicked()));
	connect(ui.btn_showSamRecs, SIGNAL(clicked()), this, SLOT(on_btnShowSamRecsClicked()));
	connect(ui.btn_debug, SIGNAL(clicked()), this, SLOT(on_btnDebugClicked()));
	connect(ui.btn_setHSV, SIGNAL(clicked()), this, SLOT(on_btnSetHSVClicked()));
	connect(ui.btn_setDataSheet, SIGNAL(clicked()), this, SLOT(on_btnSetDataSheetClicked()));

	// 下位机操作按钮
	connect(ui.btn_reset, SIGNAL(clicked()), this, SLOT(on_btnResetClicked()));
	connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(on_btnStopClicked()));

	// 动作按钮
	connect(ui.btn_recog, SIGNAL(clicked()), this, SLOT(on_btnRecogClicked()));
	connect(ui.btn_sendSingle, SIGNAL(clicked()), this, SLOT(on_btnSendSingleClicked()));
	connect(ui.btn_sendAll, SIGNAL(clicked()), this, SLOT(slot_sendOperationSerial()));

	// 可选择项
	connect(ui.inputCheckBox, SIGNAL(clicked()), this, SLOT(slot_inputStateChange()));
	connect(ui.reuseCheckBox, SIGNAL(clicked()), this, SLOT(slot_reuseStateChange()));
	
	//COM口操作相关
	connect(ui.btn_portOpen_close, SIGNAL(clicked()), this, SLOT(on_btnPortOpenClicked()));
	connect(ui.btn_portRefresh, SIGNAL(clicked()), this, SLOT(on_btnPortRefreshClicked()));
	connect(ui.btn_portSend, SIGNAL(clicked()), this, SLOT(on_btnPortSendClicked()));
	connect(ui.comboBox_coms, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_portInfoChanged(QString)));
	connect(ui.comboBox_baudRate, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_baudRateChanged()));
	connect(ui.btn_stratagyConfirm, SIGNAL(clicked()), this, SLOT(on_btnStrategyConfirm()));
	ui.comboBox_coms->clear();
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
		ui.comboBox_coms->addItem(info.portName());
	}

	//初始化摄像头并打开摄像头
	cameraInit(); 
	//on_btnCameraClicked();

	//初始化串口类对象
	serialPort = new QSerialPort();

	serialPort->setBaudRate(QSerialPort::Baud9600);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setStopBits(QSerialPort::OneStop);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);

	connect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::slot_actReadyRead);

}

CubeExplorerWithQt::~CubeExplorerWithQt() {

	writeRecords();

	if (serialPort->isOpen()) {
		serialPort->close();
		delete serialPort;
	}

	delete pMyTimer;
	delete pTimer;
	delete multiSolver;
	delete cubeExplorerSPFA;
	delete byteTmp;
	delete timeoutTimer;
	delete handReleaseDalayTimer;
}

void CubeExplorerWithQt::cameraInit()
{
	connect(ui.comboBox_cameraFR, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_cameraInfoChanged(QString)));
	connect(ui.comboBox_cameraU, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_cameraInfoChanged(QString)));
	connect(ui.comboBox_cameraBL, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_cameraInfoChanged(QString)));
	connect(ui.comboBox_cameraD, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_cameraInfoChanged(QString)));

	//摄像头显示区鼠标响应槽绑定
	connect(ui.graView_cameraFR, SIGNAL(mouseReleased(QRect)), this, SLOT(slot_mouseReleasedInCameraViews(QRect)));
	connect(ui.graView_cameraU, SIGNAL(mouseReleased(QRect)), this, SLOT(slot_mouseReleasedInCameraViews(QRect)));
	connect(ui.graView_cameraBL, SIGNAL(mouseReleased(QRect)), this, SLOT(slot_mouseReleasedInCameraViews(QRect)));
	connect(ui.graView_cameraD, SIGNAL(mouseReleased(QRect)), this, SLOT(slot_mouseReleasedInCameraViews(QRect)));

	//菜单action绑定
	connect(ui.actSetBlock, SIGNAL(triggered()), this, SLOT(slot_menuSetRecTriggered()));
	connect(ui.actShowHSV, SIGNAL(triggered()), this, SLOT(slot_menuShowHSVTriggered()));

	////获取可用摄像头信息
	//list_cameraInfo.clear();


	//ui.comboBox_cameraFR->addItem(QString::number(-1));
	//ui.comboBox_cameraU->addItem(QString::number(-1));
	//ui.comboBox_cameraBL->addItem(QString::number(-1));
	//ui.comboBox_cameraD->addItem(QString::number(-1));

	//int i = 0;
	//foreach(QCameraInfo info, QCameraInfo::availableCameras()) {
	//	list_cameraInfo.append(info);

	//	QCamera* camera_t = new QCamera(info);																//构建camera对象，存放到list_pCamera中
	//	QCameraImageCapture* capture_t = new QCameraImageCapture(camera_t);									//并构建对应于当前摄像头的capture对象，存放到list_pCapture中
	//	connect(capture_t, SIGNAL(imageSaved(int, QString)), this, SLOT(slot_imageSaved(int, QString)));	//
	//	list_pCamera.append(camera_t);
	//	list_pCapture.append(capture_t);

	//	ui.comboBox_cameraFR->addItem(QString::number(i));
	//	ui.comboBox_cameraU->addItem(QString::number(i));
	//	ui.comboBox_cameraBL->addItem(QString::number(i));
	//	ui.comboBox_cameraD->addItem(QString::number(i));

	//	i++;
	//}

	//初始化摄像头显示组件
	videoItem_FR = new QGraphicsVideoItem;//
	videoItem_FR->setSize(QSize(SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT));//
	videoItem_FR->setPos(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2);//

	videoItem_U = new QGraphicsVideoItem;//
	videoItem_U->setSize(QSize(SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT));//
	videoItem_U->setPos(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2);//

	videoItem_BL = new QGraphicsVideoItem;//
	videoItem_BL->setSize(QSize(SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT));//
	videoItem_BL->setPos(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2);//

	videoItem_D = new QGraphicsVideoItem;//
	videoItem_D->setSize(QSize(SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT));//
	videoItem_D->setPos(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2);//

	map_pic_pItem.insert("FR", videoItem_FR);//
	map_pic_pItem.insert("U", videoItem_U);//
	map_pic_pItem.insert("BL", videoItem_BL);//
	map_pic_pItem.insert("D", videoItem_D);//

	scene_FR = new QGraphicsScene(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2, SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT);//
	scene_U = new QGraphicsScene(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2, SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT);//
	scene_BL = new QGraphicsScene(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2, SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT);//
	scene_D = new QGraphicsScene(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2, SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT);//

	//在map中添加四个scene指针
	map_pic_pScene.insert("FR", scene_FR);
	map_pic_pScene.insert("U", scene_U);
	map_pic_pScene.insert("BL", scene_BL);
	map_pic_pScene.insert("D", scene_D);

	ui.graView_cameraFR->setScene(scene_FR);//
	ui.graView_cameraFR->setCursor(Qt::CrossCursor);//
	ui.graView_cameraFR->setDragMode(QGraphicsView::RubberBandDrag);//

	ui.graView_cameraU->setScene(scene_U);//
	ui.graView_cameraU->setCursor(Qt::CrossCursor);//
	ui.graView_cameraU->setDragMode(QGraphicsView::RubberBandDrag);//

	ui.graView_cameraBL->setScene(scene_BL);//
	ui.graView_cameraBL->setCursor(Qt::CrossCursor);//
	ui.graView_cameraBL->setDragMode(QGraphicsView::RubberBandDrag);//

	ui.graView_cameraD->setScene(scene_D);//
	ui.graView_cameraD->setCursor(Qt::CrossCursor);//
	ui.graView_cameraD->setDragMode(QGraphicsView::RubberBandDrag);//

	//在四个scene中分别添加视频显示组件
	scene_FR->addItem(videoItem_FR);				//
	scene_U->addItem(videoItem_U);					//
	scene_BL->addItem(videoItem_BL);				//
	scene_D->addItem(videoItem_D);					//

	//指针数组下标填充
	map_pic_cameraIndex.insert("FR", 0);	//
	map_pic_cameraIndex.insert("U", 1);		//
	map_pic_cameraIndex.insert("BL", 2);	//
	map_pic_cameraIndex.insert("D", 3);		//

	//ui.comboBox_cameraFR->setCurrentIndex(0);//
	//ui.comboBox_cameraU->setCurrentIndex(1);//
	//ui.comboBox_cameraBL->setCurrentIndex(2);//
	//ui.comboBox_cameraD->setCurrentIndex(3);//

}

// 捕获并存储相机文件
void CubeExplorerWithQt::CaptureImage() { 
	nImgSaved = 0;
	list_pCapture[map_pic_cameraIndex["FR"]]->capture();
	list_pCapture[map_pic_cameraIndex["U"]]->capture();
	list_pCapture[map_pic_cameraIndex["D"]]->capture();
	list_pCapture[map_pic_cameraIndex["BL"]]->capture();

	/*list_pCapture[map_pic_cameraIndex["FR"]]->capture(curPath + "/pic_cam/cam_" + "FR");
	list_pCapture[map_pic_cameraIndex["U"]]->capture(curPath + "/pic_cam/cam_" + "U");
	list_pCapture[map_pic_cameraIndex["D"]]->capture(curPath + "/pic_cam/cam_" + "D");
	list_pCapture[map_pic_cameraIndex["BL"]]->capture(curPath + "/pic_cam/cam_" + "BL");*/
}


void CubeExplorerWithQt::showRecogResult(std::string strRec)
{
	//strRec = ui.plainTextEdit_portWrite->toPlainText().toStdString();
	//strRec = "RRRRURRRRBBBBRBBBBDDDDFDDDDLLLLDLLLLFFFFLFFFFUUUUBUUUU";
	//strRec = "FRDFUULUDLBLLRBFRBBLFRFBDLRFDUFDFRBRLDULLUBURBDURBFDDU";
	//strRec = "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB";
	//strRec = "DDDDUUULLDRRRRLDLUFBBBFBFFFUULDDULRRRRLDLUULRFFBFBBBFB";

	//CreateSceneFromStr(strRec);

	//把识别结果显示在界面上
	QGraphicsScene* scene_rec_F = new QGraphicsScene;
	QGraphicsScene* scene_rec_R = new QGraphicsScene;
	QGraphicsScene* scene_rec_U = new QGraphicsScene;
	QGraphicsScene* scene_rec_B = new QGraphicsScene;
	QGraphicsScene* scene_rec_L = new QGraphicsScene;
	QGraphicsScene* scene_rec_D = new QGraphicsScene;
	QImage* image = new QImage();
	QImage imageTmp;

	// 在窗体上生成识别结果窗口
	image->load(QString("./pic_res/res_r.png"));
	imageTmp = image->scaled(ui.graphicsView_R->width() - 10, ui.graphicsView_R->height() - 10);
	scene_rec_R->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_R->setScene(scene_rec_R);
	ui.graphicsView_R->show();

	image->load(QString("./pic_res/res_f.png"));
	imageTmp = image->scaled(ui.graphicsView_F->width() - 10, ui.graphicsView_F->height() - 10);
	scene_rec_F->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_F->setScene(scene_rec_F);
	ui.graphicsView_F->show();

	image->load(QString("./pic_res/res_u.png"));
	imageTmp = image->scaled(ui.graphicsView_U->width() - 10, ui.graphicsView_U->height() - 10);
	scene_rec_U->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_U->setScene(scene_rec_U);
	ui.graphicsView_U->show();

	image->load(QString("./pic_res/res_b.png"));
	imageTmp = image->scaled(ui.graphicsView_B->width() - 10, ui.graphicsView_B->height() - 10);
	scene_rec_B->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_B->setScene(scene_rec_B);
	ui.graphicsView_B->show();

	image->load(QString("./pic_res/res_l.png"));
	imageTmp = image->scaled(ui.graphicsView_L->width() - 10, ui.graphicsView_L->height() - 10);
	scene_rec_L->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_L->setScene(scene_rec_L);
	ui.graphicsView_L->show();

	image->load(QString("./pic_res/res_d.png"));
	imageTmp = image->scaled(ui.graphicsView_D->width() - 10, ui.graphicsView_D->height() - 10);
	scene_rec_D->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_D->setScene(scene_rec_D);
	ui.graphicsView_D->show();
}

void CubeExplorerWithQt::Sleep(int sec) {
	QDateTime last = QDateTime::currentDateTime();
	QDateTime now;
	while (1) {
		now = QDateTime::currentDateTime();
		if (last.msecsTo(now) >= sec) {
			break;
		}
	}
}

void CubeExplorerWithQt::initRecords() {
	//从本地文件读取存储的HSV数据？？？？这里读取的内容存疑
	QFile file_record(QDir::currentPath() + "/data/restoreRecord.txt");	//读取阈值数据文件到 str_record 以进行分割、遍历
	file_record.open(QIODevice::ReadWrite);									//
	QTextStream ts(&file_record);											//
	QString str_record = ts.readAll();									//


	QList<QString> list_recordStr = str_record.split('_');			//根据 '_' 分割得到复原步数记录

	for (int i = 0; i < list_recordStr.size() - 1; i += 2) {		
		int cnt = list_recordStr[i].toInt();
		double time = list_recordStr[i + 1].toDouble();
		list_restoreRecord.push_back(RestoreRecord(cnt,time));
	}

	file_record.close();
}

void CubeExplorerWithQt::writeRecords()
{
	QString data2write;
	for (int i = 0; i < list_restoreRecord.size(); i++) {
		data2write += QString::asprintf("%d_%f", list_restoreRecord[i].cnt, list_restoreRecord[i].time);
		if (i != list_restoreRecord.size() - 1) {
			data2write += '_';
		}
	}
	QFile::remove(QDir::currentPath() + "/Data/restoreRecord.txt");			//直接删除原来的数据文件
	QFile file_record(QDir::currentPath() + "/Data/restoreRecord.txt");		//创建并打开新文件进行输入
	file_record.open(QIODevice::WriteOnly);									//
	QTextStream ts(&file_record);											//
	ts << data2write;														//
	file_record.close();				
}

void CubeExplorerWithQt::slot_sendOperationSerial() {
/*
		const string commandOp[29] = {
		"#1P6T75\r\n", "#1P6T50\r\n", "#1P6T100\r\n",
		"#1P8T50\r\n", "#1P8T25\r\n", "#1P8T100\r\n",
		"#1P7T75\r\n", "#1P7T50\r\n", "#1P7T100\r\n",
		"#2P0T75\r\n", "#2P1T75\r\n",
		"#1P9T200\r\n","#1P9T200\r\n","#1P9T200\r\n",

		"#3P6T75\r\n", "#3P6T50\r\n", "#3P6T100\r\n",
		"#3P8T50\r\n", "#3P8T25\r\n", "#3P8T100\r\n",
		"#3P7T75\r\n", "#3P7T50\r\n", "#3P7T100\r\n",
		"#4P0T75\r\n", "#4P1T75\r\n",
		"#3P9T200\r\n", "#3P9T200\r\n", "#3P9T200\r\n"
	};
*/
	serialPort->write(QString("#2P1T75\r\n").toLatin1());
	serialPort->write(QString("#4P1T75\r\n").toLatin1());
	for (auto iter = cubeExplorerSPFA->GetVecStrSerial().cbegin(); iter != cubeExplorerSPFA->GetVecStrSerial().cend(); iter++) {
		serialPort->write(QString(iter->c_str()).toLatin1());
	}
	cubeExplorerSPFA->GetVecStrSerial().clear();
	//serialPort->write(QString("#2P0T200\r\n").toLatin1());
	serialPort->write(QString("#4P0T200\r\n").toLatin1());
	serialPort->write(QString("#7P0T200\r\n").toLatin1());
	serialPort->flush();
}

//优化解算方法
void CubeExplorerWithQt::on_btnSendSingleClicked() { // 单步执行解算

	/*serialPort->write(QString("#4P1T200\r\n").toLatin1());*/
//	serialPort->write(QString("#5P0T200\r\n").toLatin1());

	if (cubeExplorerSPFA->GetVecStrSerial().empty()) {
		serialPort->write(QString("#2P0T200\r\n").toLatin1());
		serialPort->write(QString("#4P0T200\r\n").toLatin1());
		serialPort->write(QString("#7P0T200\r\n").toLatin1());
		serialPort->flush();
		return;
	}
	serialPort->write(QString(cubeExplorerSPFA->GetVecStrSerial()[0].c_str()).toLatin1());
	cubeExplorerSPFA->GetVecStrSerial().erase(cubeExplorerSPFA->GetVecStrSerial().begin());
}

//--------------------------------------------------------------------------------------
//右边栏操作按钮响应槽函数
void CubeExplorerWithQt::on_btnTightOrLooseClicked() {
	//操作两只机械手进行微松开和夹紧，用以装配和取下魔方
	if (cubeExplorerSPFA->hand_state.left_is_tight) {
		serialPort->write(QString("#2P0T200\r\n").toLatin1());
		serialPort->write(QString("#4P0T200\r\n").toLatin1());
		cubeExplorerSPFA->hand_state = Hand_State(false, false);
	}
	else {
		serialPort->write(QString("#2P1T200\r\n").toLatin1());
		serialPort->write(QString("#4P1T200\r\n").toLatin1());
		cubeExplorerSPFA->hand_state = Hand_State(true, true);
	}
}

void CubeExplorerWithQt::on_btnResetClicked() {
	//操作夹子张开，两只手臂恢复到初始角度并张开，夹子合拢

//	serialPort->write(QString("#5P0T200\r\n").toLatin1()); // clamp_open

	serialPort->write(QString("#2P0T200\r\n").toLatin1());
	serialPort->write(QString("#4P0T200\r\n").toLatin1());
	bRestore = false;

	/*if (!cubeExplorer.handState.left.isReady) {
		serialPort->write(QString("#1P90T200\r\n").toLatin1());
		cubeExplorer.handState.left.isReady = true;
	}
	if (!cubeExplorer.handState.right.isReady) {
		serialPort->write(QString("#3P90T200\r\n").toLatin1());
		cubeExplorer.handState.right.isReady = true;
	}
	if (cubeExplorer.handState.left.isTight) {
		serialPort->write(QString("#2P0T200\r\n").toLatin1());
		serialPort->write(QString("#4P0T200\r\n").toLatin1());
		cubeExplorer.handState = HandState(true, false, true, false);
	}*/
//	serialPort->write(QString("#5P1T200\r\n").toLatin1()); // clamp_close
}

void CubeExplorerWithQt::on_btnRestoreClicked() {
	pTimer->setInterval(50), pMyTimer->reset();
	pTimer->start(), pMyTimer->start();
	serialPort->write(QString("#2P1T75\r\n").toLatin1());
	serialPort->write(QString("#4P1T75\r\n").toLatin1());
	serialPort->flush();
	bRestore = true; CaptureImage();
	//continueRestore();
}
#define REALRU
void CubeExplorerWithQt::continueRestore()
{
	//2.进行识别得到识别字符串
	std::string strRec = "";
	char* cp; char* res;

#ifdef REALRUN
	strRec = recognizeNew();
#endif // REALRUN

#ifndef REALRUN
	if (inputFromBox)
		strRec = ui.plainTextEdit_portWrite->toPlainText().toStdString();
	else
		strRec = recognizeNew();
#endif // !REALRUN

	/*ui.plainTextEdit_portWrite->setPlainText("test\n" + QString::number(strRec.length()) + "\n" + strRec.c_str());*/
	//strRec = "UUUUBUUUURRRRURRRRFFFFLFFFFDDDDFDDDDLLLLDLLLLBBBBRBBBB";
	//strRec = "UDUDUDUDURRRRRRRRRFFFFFFFFFDUDUDUDUDLLLLLLLLLBBBBBBBBB";

	double st = clock(), ed;
	cp = new char[strRec.length() + 1];
	ui.plainTextEdit_portWrite->setPlainText("识别中");
	strcpy(cp, strRec.c_str()); res = CubeSolver(cp, NULL);

	//std::string tmp = "";
	//tmp += "RecogResult: " + strRec + "\r\n     Solve6: ";
	//ui.plainTextEdit_portWrite->setPlainText(tmp.c_str());
	//res = ui.plainTextEdit_portWrite->toPlainText().toStdString();
	//res = "U2L3B3U2R3U3L2B2U1R3R2D3R2U1R2U3F2U1F2U3F2U2L2F2R2F2D2L2D2L2B2";
	if (!res) {
		showRecogResult(strRec);
		ui.label_UI_message->setText(QStringLiteral("识别序列有误！"));
		ui.plainTextEdit_portWrite->setPlainText(QStringLiteral("识别序列有误！"));
		pTimer->stop(); //停止计时器
		bResponseStart = true;
#ifdef REALRUN
		saveCaptureMat(curPath.toStdString() + "/pic_cam/cam_");
#endif // 
		return;
	}
	cubeExplorerSPFA->GetShortestPath(res);
	if (cubeExplorerSPFA->GetAnsOpStepNumber() > 77) {
		cubeExplorerSPFA = multiSolver->GetMultiThreadPath(strRec);
	}
	else cubeExplorerSPFA->SaveMechanicalStep();

	/*cubeExplorerSPFA->GetShortestPath(res);
	cubeExplorerSPFA->SaveMechanicalStep();*/
	//5.通过串口通信把串口序列传递给控制机，并使用listView控件实时显示操作序列传输情况
	if (bRestore) slot_sendOperationSerial();
	else {
		serialPort->write(QString("#2P0T200\r\n").toLatin1());
		serialPort->write(QString("#4P0T200\r\n").toLatin1());
	}
	ed = clock();
	std::string strDisplay = "";
	
	showRecogResult(strRec);
	ui.label_UI_message->setText(QStringLiteral("识别正确！"));
	strDisplay += "RecogResult: " + strRec + "\r\n      Solve6: " + res;
	//ui.plainTextEdit_portWrite->setPlainText(QString("RecogResult: ") + strRec.c_str() + QString("\r\n      ") + strDisplay.c_str());

	std::string ansOpSequence = cubeExplorerSPFA->GetAnsOpSequence();
	int steps = cubeExplorerSPFA->GetAnsOpStepNumber();

	strDisplay += "\r\n      Solve2: " + ansOpSequence;
	strDisplay += "\r\n      AnsCostTime: " + to_string(cubeExplorerSPFA->GetAnsCostTime());
	strDisplay += (cubeExplorerSPFA->reuseFlag == true) ? " (True)" : " (False)";
	strDisplay += "\r\n      Total: " + to_string(steps) + " steps" + "\r\n      Time: " + to_string(int(ed - st)) + "ms";
	
	ui.plainTextEdit_portWrite->setPlainText(strDisplay.c_str());
	ui.label_restoreCnt->setText(to_string(steps).c_str());

#ifdef REALRUN
	saveCaptureMat(curPath.toStdString() + "/pic_cam/cam_");
#endif // 
}

cv::Mat CubeExplorerWithQt::QImageToCvMat(const QImage& image) {
	cv::Mat mat;
	switch (image.format()) {
	case QImage::Format_RGB888:
		mat = cv::Mat(image.height(), image.width(), CV_8UC3,
			const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();
		cv::cvtColor(mat, mat, cv::COLOR_RGB2BGR);
		break;
	case QImage::Format_RGB32:
	case QImage::Format_ARGB32:
	case QImage::Format_ARGB32_Premultiplied:
		mat = cv::Mat(image.height(), image.width(), CV_8UC4, 
			(void*)image.constBits(), image.bytesPerLine()).clone();
		/*mat = cv::Mat(image.height(), image.width(), CV_8UC4,
			const_cast<uchar*>(image.bits()), image.bytesPerLine()).clone();*/
		break;
	case QImage::Format_Grayscale8:
		mat = cv::Mat(image.height(), image.width(),
			CV_8UC1, const_cast<uchar*>(image.bits()),
			image.bytesPerLine()).clone();
		break;
	default:
		qWarning() << "QImage format not handled in switch:" << image.format();
		break;
	}
	return mat;
}

void CubeExplorerWithQt::saveCaptureMat(string curPath)
{
	for (int i = 0; i < 4; i++)
		cv::imwrite(curPath + list_picID[i].toStdString() + ".jpg", captureMatSet[i]);
}

void CubeExplorerWithQt::on_btnDebugClicked() {
	DebugWidget dw(serialPort, this);
	dw.setWindowTitle(QStringLiteral("调试"));
	dw.show();
	dw.exec();
}

void CubeExplorerWithQt::on_btnCameraClicked() {
	static bool cameraOpenFlag = false;
	if (cameraOpenFlag) return;
	cameraOpenFlag = true;

	//刷新可用摄像头信息（打开摄像头开关）
	list_cameraInfo.clear();
	list_pCamera.clear();
	list_pCapture.clear();
	//list_pSnap.clear();

	ui.comboBox_cameraFR->clear();
	ui.comboBox_cameraU->clear();
	ui.comboBox_cameraBL->clear();
	ui.comboBox_cameraD->clear();

	ui.comboBox_cameraFR->addItem(QString::number(-1));
	ui.comboBox_cameraU->addItem(QString::number(-1));
	ui.comboBox_cameraBL->addItem(QString::number(-1));
	ui.comboBox_cameraD->addItem(QString::number(-1));
	
	int i = 0;
	foreach(QCameraInfo info, QCameraInfo::availableCameras()) {
		list_cameraInfo.append(info);

		QCamera* camera_t = new QCamera(info);																//构建camera对象，存放到list_pCamera中
		QCameraImageCapture* capture_t = new QCameraImageCapture(camera_t);									//并构建对应于当前摄像头的capture对象，存放到list_pCapture中
		//QVideoProbe* snap_t = new QVideoProbe(this);
		//snap_t->setSource(camera_t);

		capture_t->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
		//connect(capture_t, SIGNAL(imageSaved(int, QString)), this, SLOT(slot_imageSaved(int, QString)));	//
		connect(capture_t, &QCameraImageCapture::imageCaptured, this, &CubeExplorerWithQt::slot_imageCaptured);	//
		map_capture_pId[capture_t] = i;

		list_pCamera.append(camera_t);																		//
		list_pCapture.append(capture_t);																	//																//
		//list_pSnap.append(snap_t);

		ui.comboBox_cameraFR->addItem(QString::number(i)); // 这里设置了四个摄像头，以前是三个
		ui.comboBox_cameraU->addItem(QString::number(i));
		ui.comboBox_cameraBL->addItem(QString::number(i));
		ui.comboBox_cameraD->addItem(QString::number(i));
		i++;
	}

	if (list_pCamera.size() >= 1) list_pCamera[0]->setViewfinder(videoItem_FR);	//
	if (list_pCamera.size() >= 2) list_pCamera[1]->setViewfinder(videoItem_U);	//
	if (list_pCamera.size() >= 3) list_pCamera[2]->setViewfinder(videoItem_BL);	//
	if (list_pCamera.size() >= 4) list_pCamera[3]->setViewfinder(videoItem_D);	//
	
	// 设置下拉框默认选项，为下拉框中数字索引（start from 1: 0是-1
	ui.comboBox_cameraFR->setCurrentIndex(1);
	ui.comboBox_cameraU->setCurrentIndex(2);
	ui.comboBox_cameraBL->setCurrentIndex(3);
	ui.comboBox_cameraD->setCurrentIndex(4);

	QCameraViewfinderSettings set; // 设置摄像头刷新率30hz, 分辨率1920*1080
	set.setMaximumFrameRate(30);
	set.setMinimumFrameRate(30);
	set.setResolution(1920,1080);
	for (int i = 0; i < list_pCamera.length(); i++) {
		if (list_pCamera[i]->status() != QCamera::ActiveStatus) list_pCamera[i]->start();
		list_pCamera[i]->setViewfinderSettings(set);
	}
}

void CubeExplorerWithQt::on_btnShowSamRecsClicked(){
	//显示采样框
	if (!list_samRecItems.isEmpty()) { //再次点击清除已显示采样框
		foreach(QGraphicsRectItem* pItem, list_samRecItems) {
			pItem->scene()->removeItem(pItem);
			delete pItem;
		}
		list_samRecItems.clear();
		return;
	}
	QMap<QString, vector<SamRec>> &map_pic_id_samRec = getSamRecMap();	//获取采样框数据map
	vector<SamRec> map_id_samRec;
	QGraphicsScene* scene;
	QRect rect;
	QGraphicsRectItem *item;

	QPen pen;	//自定义画笔进行item的绘画
	pen.setWidth(2);	//
	pen.setColor(Qt::green);	//

	for (int i = 0; i < map_pic_id_samRec.size(); i++) {				//遍历采样框数据map
		map_id_samRec = map_pic_id_samRec[list_picID[i]];				//从list_picID获取字符串作为键值从采样框数据map中获取对应图片的采样框
		scene = map_pic_pScene[list_picID[i]];							//以同样的键值从scene指针map中获取对应图片的scene指针
		for (int j = 0; j < map_id_samRec.size(); j++) {				//遍历单个图片的采样框数据vector,将SamRec结构转换为符合视野比例的QRect并利用scene指针将采样框绘制到界面上
			rect = { map_id_samRec[j].x1 / 2 - SCENE_VIEW_WIDTH/2,		//x = x1 / 2	（-scene宽度的一半是为了映射到scene坐标系）
					 map_id_samRec[j].y1 / 2 - SCENE_VIEW_HEIGHT/2,		//y = y1 / 2	（-scene高度的一半是为了映射到scene坐标系）
					(map_id_samRec[j].x2 - map_id_samRec[j].x1) / 2,	//width = (x2 - x1) / 2
					(map_id_samRec[j].y2 - map_id_samRec[j].y1) / 2 };	//height = (y2 - y1) / 2
			
			item = new QGraphicsRectItem(rect);							//构建RectItem并绘制到指定scene
			item->setPen(pen);											//
			scene->addItem(item);										//
			list_samRecItems.append(item);								//保存这些矩形的指针，方便进行清除
		}
	}/**/

}

void CubeExplorerWithQt::on_btnRecogClicked() {
	bRestore = false;

#ifdef REALRUN
	CaptureImage();
#endif // REALRUN

#ifndef REALRUN
	if (inputFromBox) continueRestore();
	else CaptureImage();
#endif // !REALRUN
}

void CubeExplorerWithQt::on_btnSetHSVClicked(){
	//HSV设置
	HSVThresholdDialog td(this);
	td.setWindowTitle(QStringLiteral("HSV阈值设置"));
	td.show(); td.exec();
}

void CubeExplorerWithQt::on_btnStopClicked()
{
	serialPort->write("#S\n\r");
}

void CubeExplorerWithQt::on_btnSetDataSheetClicked()
{
	disconnect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::slot_comReadyRead);
	DataSheetWidget dsw(serialPort, this);
	dsw.setWindowTitle(QStringLiteral("数据表设置"));
	dsw.show();
	dsw.exec();
	connect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::slot_comReadyRead);
}

//右键菜单响应槽函数
void CubeExplorerWithQt::slot_mouseReleasedInCameraViews(QRect rec_select)
{
	rec_tSelect = rec_select;

	QMyGraphicsView *view_sender = qobject_cast<QMyGraphicsView*>(sender()); //摄像头框
	QPointF sceneAxis_startPoint = view_sender->mapToScene(rec_select.x(), rec_select.y());	//将View坐标（摄像头框内）映射到Scene坐标（屏幕上）
	QGraphicsRectItem* item = new QGraphicsRectItem(QRect(sceneAxis_startPoint.x(), sceneAxis_startPoint.y(), rec_select.width(), rec_select.height())); //摄像头框的矩形
	QPen pen;						//自定义画笔进行item的绘画
	pen.setWidth(2);				//
	pen.setColor(Qt::black);		//
	item->setPen(pen);				//
	view_sender->scene()->addItem(item);	//绘制item到场景内

	//弹出菜单
	ui.actSetBlock->setData(view_sender->objectName());	//弹出菜单-设置取色块：配置action数据，告知当前触发菜单的摄像头View
	ui.actShowHSV->setData(view_sender->objectName());	//弹出菜单-查看HSV数据

	QMenu *popMenu = new QMenu();
	popMenu->addAction(ui.actSetBlock);
	popMenu->addAction(ui.actShowHSV);
	popMenu->addSeparator();
	popMenu->addAction(ui.actCancel);
	popMenu->exec(QCursor::pos());
	view_sender->scene()->removeItem(item);
	delete popMenu;
	popMenu = nullptr;
}

void CubeExplorerWithQt::slot_menuSetRecTriggered()
{
	QString t = ui.actSetBlock->data().toString();
	QString caller;
	if (t[t.length() - 2] != "a") caller.append(t[t.length() - 2]);
	caller.append(t[t.length() - 1]);

	RecogAreaDialog rad(caller, rec_tSelect, this);
	rad.setWindowTitle("SetRec");
	rad.show();
	rad.exec();
}

void CubeExplorerWithQt::slot_menuShowHSVTriggered()
{
	QString t = ui.actShowHSV->data().toString();
	QString caller;
	if (t[t.length() - 2] != "a") caller.append(t[t.length() - 2]);
	caller.append(t[t.length() - 1]);

	list_pCapture[map_pic_cameraIndex[caller]]->capture(curPath+"/pic_cam/"+caller+"_temp_for_showHSV");
}

void CubeExplorerWithQt::slot_setRecArea(QString groupName,QRect rect,int faceID, int blockID)
{
	ui.label_UI_message->setText(QStringLiteral("set Rect for") + groupName + faceID + blockID);
	SamRec rec2set = { (rect.x() - 5) * 2,(rect.x() + rect.width() - 5) * 2,(rect.y() - 5) * 2,(rect.y() + rect.height() - 5) * 2 };
	setSampleRec(groupName, rec2set, faceID, blockID);		//设置采样矩阵

}



void CubeExplorerWithQt::slot_imageSaved(int id, QString fileName)
{
	//if (fileName.indexOf("HSV")!=-1) {				//确认当前操作是统计HSV数值
	//	cv::Mat mat_t = cv::imread(fileName.toStdString());
	//	cv::resize(mat_t, mat_t, cv::Size(640, 320));
	//	int x = (rec_tSelect.x() - 5) * 2;
	//	int y = (rec_tSelect.y() - 5) * 2;
	//	cv::Mat imgBGR = mat_t(cv::Range(y, y + rec_tSelect.height() * 2), cv::Range(x, x + rec_tSelect.width() * 2));
	//	HSVDataDialog HSVDialog(imgBGR, this);
	//	HSVDialog.setWindowTitle(QStringLiteral("HSV数据统计"));
	//	HSVDialog.setMinimumWidth(700);
	//	HSVDialog.show();
	//	HSVDialog.exec();
	//}
	//else 
	//	nImgSaved++;								//拍照计数器递增，用于在复原前判断截图保存是否完成
	if (++nImgSaved == 4) continueRestore();

//策略1--------------------------------------------------------------------------------
	//if (nImgSaved == 3) {

	//	//c.右手夹紧，左手松开，为下次拍照做好准备
	//	serialPort->write(QString("#4P1T200\r\n").toLatin1());
	//	serialPort->write(QString("#2P5T200\r\n").toLatin1());
	//	serialPort->flush();
	//	cubeExplorer.handState.left.isTight = false;
	//	cubeExplorer.handState.right.isTight = true;
	//	Sleep(captureInterval);

/*		capture("case2");	*/						//三张图片保存完毕，case1拍照成功，进行case2的拍照
	//}
	//if (nImgSaved == 6) continueRestore();			//六张图片保存完毕，case2拍照成功，发送信号进行后续复原操作
//-------------------------------------------------------------------------------------

}

void CubeExplorerWithQt::slot_imageCaptured(int id, const QImage& image)
{
	QCameraImageCapture* senderCapture = qobject_cast<QCameraImageCapture*>(sender());
	int pId = map_capture_pId[senderCapture];

	QImage pressImg = image.scaled(640, 360, Qt::IgnoreAspectRatio);

	captureMatSet[pId] = QImageToCvMat(image.scaled(640, 360, Qt::IgnoreAspectRatio));

	//string filename = curPath.toStdString() + "/pic_cam/cam_" + list_picID[pId].toStdString() + "_raw.jpg";
	//image.save(filename.c_str());
	//filename = curPath.toStdString() + "/pic_cam/cam_" + list_picID[pId].toStdString() + "_press.jpg";
	//pressImg.save(filename.c_str());
	//ofstream out;
	//out.open("captest.txt", std::ios::app);
	//out << "current id = " << id << endl;
	//out << "raw format: " << image.format() << " press format: " << pressImg.format()<< " QImage::Format_RGB888:"<< QImage::Format_RGB888 << endl;
	//out << filename<< " " << pId << endl;
	//out.close();
	//cv::imwrite(curPath.toStdString() + "/pic_cam/cam_" + list_picID[pId].toStdString() + ".jpg", captureMatSet[pId]);
	//
	sampleFromPic(captureMatSet[pId], list_picID[pId]);
	if (++nImgSaved == 4) continueRestore();
}

//串口模块响应槽函数
void CubeExplorerWithQt::on_btnPortRefreshClicked()
{
	ui.comboBox_coms->clear();
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
		ui.comboBox_coms->addItem(info.portName());
	}
}

void CubeExplorerWithQt::on_btnPortOpenClicked() // 打开或关闭串口
{
	if (serialPort->isOpen()) {
		serialPort->close();
		ui.label_portMessage->setText(QStringLiteral("已关闭串口..."));
	}
	else {
		if (serialPort->open(QIODevice::ReadWrite)) ui.label_portMessage->setText(QStringLiteral("打开串口成功..."));
		else ui.label_portMessage->setText(QStringLiteral("打开串口失败..."));
	}
}

void CubeExplorerWithQt::on_btnPortSendClicked() // 向串口发送输入框内信息
{
	if (serialPort->isOpen()) {
		serialPort->write(ui.plainTextEdit_portWrite->toPlainText().toLatin1());
		serialPort->flush();
	}
	else ui.label_portMessage->setText(QStringLiteral("串口未打开！！！"));
}

void CubeExplorerWithQt::slot_portInfoChanged(const QString & text) {
	serialPort->setPortName(text);
}

void CubeExplorerWithQt::slot_cameraInfoChanged(const QString & text)
{
	int index = text.toInt();
	if(!(~index)) return;

	QString str_t = sender()->objectName();
	//ui.plainTextEdit_portWrite->setPlainText(str_t);

	QString picName = QString(str_t[str_t.length() - 2])=="a" ? QString(str_t[str_t.length() - 1]):QString(str_t[str_t.length() - 2]) + QString(str_t[str_t.length() - 1]);
	list_pCamera[index]->setViewfinder(map_pic_pItem[picName]);

	map_pic_cameraIndex.insert(picName, index);
}

void CubeExplorerWithQt::slot_timeout() {
	double second = pMyTimer->getTime();
	ui.lineEdit_second1->setText(QString::asprintf("%d", int(second)));
	ui.lineEdit_second2->setText(QString::asprintf("%d", int((second-int(second))*100)));
}

void CubeExplorerWithQt::slot_onReceiveTimeout()
{
	//QByteArray byteTmp=serialPort->readAll();
	//serialPort->clear(QSerialPort::Input);
	if (!byteTmp->isEmpty()) {
		QString decodedString = QTextCodec::codecForName("GB18030")->toUnicode(*byteTmp);
		ui.plainTextEdit_SerialRX->insertPlainText(decodedString);

		////QString StrbyteTmp = QString(byteTmp);
		////ui.plainTextEdit_SerialRX->insertPlainText(StrbyteTmp);
		//if (byteTmp->contains("Sta") && bRestore == false) {//
		//	//接收到开始按钮指令，开始复原
		//	on_btnRestoreClicked();
		//	ui.label_UI_message->setText(QStringLiteral("串口收到开始信号"));
		//}
		//else if (byteTmp->contains("Ove")) {
		//	pTimer->stop();
		//	//int cnt = cubeExplorer.transCnt;
		//	double time = double(int(pMyTimer->getTime() * 100)) / 100;
		//	//list_restoreRecord.push_back(RestoreRecord(cnt, time));
		//	ui.label_UI_message->setText(QStringLiteral("串口收到结束信号"));
		//	bResponseStart = true;
		//}
		//else if (byteTmp->contains("Reset")) {
		//	//接收到开机指令
		//	bResponseStart = true;
		//}/*
		//else if (byteTmp.contains("Pause")) {
		//	bResponseStart = true;
		//}*/
		byteTmp->clear();
	}
}

void CubeExplorerWithQt::slot_inputStateChange()
{
	if (inputFromBox) ui.label_inputState->setText(QStringLiteral("已取消输入框输入"));
	else ui.label_inputState->setText(QStringLiteral("请从输入框输入"));
	inputFromBox^=true;
		
}

void CubeExplorerWithQt::slot_reuseStateChange()
{
	cubeExplorerSPFA->reuseFlag ^= true;
	if(cubeExplorerSPFA->reuseFlag)
		ui.label_inputState->setText(QStringLiteral("已开启时间复用"));
	else ui.label_inputState->setText(QStringLiteral("已关闭时间复用"));
}

void CubeExplorerWithQt::slot_comReadyRead()
{
	QByteArray newData = serialPort->readAll();
	serialPort->clear(QSerialPort::Input);
	byteTmp->append(newData);
	timeoutTimer->start(100);
}

void CubeExplorerWithQt::slot_actReadyRead()
{
	QByteArray comByteBuffer = serialPort->readAll();	serialPort->clear(QSerialPort::Input);
	if (!comByteBuffer.isEmpty()) {

		ui.plainTextEdit_SerialRX->insertPlainText(QString::fromLatin1(comByteBuffer));
		ui.plainTextEdit_SerialRX->ensureCursorVisible();        // 确保光标可见
		
		if (comByteBuffer.contains("Sta") && bResponseStart) {//
		//接收到开始按钮指令，开始复原
			bResponseStart = false;
			on_btnRestoreClicked();
			ui.label_UI_message->setText(QStringLiteral("串口收到开始信号"));
		}
		else if (comByteBuffer.contains("Ove")) {
			pTimer->stop(); bRestore = false;
			//int cnt = cubeExplorer.transCnt;
			double time = double(int(pMyTimer->getTime() * 100)) / 100;
			//list_restoreRecord.push_back(RestoreRecord(cnt, time));
			ui.label_UI_message->setText(QStringLiteral("串口收到结束信号"));
			bResponseStart = true;
			handReleaseDalayTimer->start(500);
		}
		else if (comByteBuffer.contains("Reset")) {
			bResponseStart = true;
		}
		else if (comByteBuffer.contains("Relax")) {
			bResponseStart = true;
		}
	}
}

void CubeExplorerWithQt::slot_comBufferProcessor()
{

}

void CubeExplorerWithQt::slot_baudRateChanged()
{
	int baudRate = ui.comboBox_baudRate->currentText().toInt();
	serialPort->setBaudRate(baudRate);
	QString strDis = QStringLiteral("当前波特率：") + to_string(baudRate).c_str();
	ui.plainTextEdit_portWrite->setPlainText(strDis);
	ui.plainTextEdit_SerialRX->clear();
	switch (baudRate) {
	case 9600:
		disconnect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::slot_comReadyRead);
		connect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::slot_actReadyRead);
		break;
	case 115200:
		disconnect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::slot_actReadyRead);
		connect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::slot_comReadyRead);
		break;
	}
}

void CubeExplorerWithQt::on_btnStrategyConfirm()
{
	if (!serialPort->isOpen()) {
		ui.label_portMessage->setText(QStringLiteral("串口未打开！！！"));
		return;
	}

	string strategy = ui.comboBox_strategy->currentText().toStdString();

	switch (strategy[0]) {
	case 's':
		serialPort->write("\r\n");
		serialPort->write("set_rotate_rise_speed_180 130000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed 130000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed_close 150000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed_close_1 150000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_speed_180 140000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_90 140000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_close 140000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_close_1 140000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		/*serialPort->write(QString("set_continu_rotate_speed 200000\r\n").toLatin1());
		serialPort->write(QString("set_Positioning_speed 5000\r\n").toLatin1());*/
		serialPort->write(QString("set_rotate_over_delay 10\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_open_over_delay 10\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_close_over_delay 10\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		break;
	case 'm':
		serialPort->write("\r\n");
		serialPort->write("set_rotate_rise_speed_180 150000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed 150000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed_close 160000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed_close_1 160000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_speed_180 200000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_90 200000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_close 200000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_close_1 200000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		/*serialPort->write(QString("set_continu_rotate_speed 200000\r\n").toLatin1());
		serialPort->write(QString("set_Positioning_speed 5000\r\n").toLatin1());*/
		serialPort->write(QString("set_rotate_over_delay 10\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_open_over_delay 15\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_close_over_delay 10\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		break;
	case 'f':
		serialPort->write("\r\n");
		serialPort->write("set_rotate_rise_speed_180 160000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed 160000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed_close 160000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_rise_speed_close_1 160000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write("set_rotate_speed_180 200000\r\n");
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_90 220000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_close 220000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_close_1 220000\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		/*serialPort->write(QString("set_continu_rotate_speed 200000\r\n").toLatin1());
		serialPort->write(QString("set_Positioning_speed 5000\r\n").toLatin1());*/
		serialPort->write(QString("set_rotate_over_delay 5\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_open_over_delay 20\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_close_over_delay 5\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		break;
	case 'u':
		serialPort->write("\r\n");
		serialPort->write(QString("set_rotate_rise_speed_180 150000\r\n").toLatin1());  		//  set_rotate_rise_speed_180 - 设置手爪翻转180旋转升降速度      带魔方整体翻转180度的加速度
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_rise_speed 150000\r\n").toLatin1());		        //	set_rotate_rise_speed - 设置手爪翻转90旋转升降速度           带魔方整体翻转90度的加速度
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_rise_speed_close 190000\r\n").toLatin1());		//	set_rotate_rise_speed_close - 设置对面手爪闭合时旋转升降速度  空转 + 拧魔方翻转90 + 180度的加速度
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_rise_speed_close_1 190000\r\n").toLatin1());		//	set_rotate_rise_speed_close_1 - 设置对面手爪闭合时连续旋转升降速度 ？？？连续转90 / 180度的第二次
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_180 180000\r\n").toLatin1());				//	set_rotate_speed_180 - 设置手爪翻转180旋转速度  带魔方整体翻转180度的最大速度
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_90 180000\r\n").toLatin1());				//	set_rotate_speed_90 - 设置手爪翻转90旋转速度 带魔方整体翻转90度的最大速度
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_close 190000\r\n").toLatin1());				//	set_rotate_speed_close - 设置对面手爪闭合时手爪旋转速度 空转 + 拧魔方翻转90 + 180度的最大速度
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_rotate_speed_close_1 190000\r\n").toLatin1());			//	set_rotate_speed_close_1 - 设置对面手爪闭合时手爪连续旋转速度   ？？？连续转90 / 180度的第二次的最大速度
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		/*serialPort->write(QString("set_continu_rotate_speed 200000\r\n").toLatin1());
		serialPort->write(QString("set_Positioning_speed 5000\r\n").toLatin1());*/
		serialPort->write(QString("set_rotate_over_delay 5\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_open_over_delay 8\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		serialPort->write(QString("set_close_over_delay 8\r\n").toLatin1());
		serialPort->waitForBytesWritten();
		QThread::msleep(100);
		break;

	}
	serialPort->write("print_para\r\n");
	serialPort->flush();
}