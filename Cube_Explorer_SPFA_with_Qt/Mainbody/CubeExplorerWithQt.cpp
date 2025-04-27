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
	pTimer->setInterval(50); //设置计时器间隔，用于刷新显示复原用时
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
	connect(handReleaseDalayTimer, &QTimer::timeout, this, [this]() {
		ui.plainTextEdit_SerialRX->appendPlainText("松手");
		serialPort->write(QString("#2P0T200\r\n").toLatin1());
		serialPort->write(QString("#4P0T200\r\n").toLatin1());
		isToRestore = false;
	});

	/*connect(ui.btn_tightOrLoose, SIGNAL(clicked()), this, SLOT(on_btnTightOrLooseClicked()));*/
	
	// 开始复原按钮
	connect(ui.btn_restore, SIGNAL(clicked()), this, SLOT(on_btnRestoreClicked()));

	// 普通操作按钮
	connect(ui.btn_camSwitch, SIGNAL(clicked()), this, SLOT(onbtnCamSwitchClicked()));
	connect(ui.btn_showSamRecs, SIGNAL(clicked()), this, SLOT(on_btnShowSamRecsClicked()));
	connect(ui.btn_debug, SIGNAL(clicked()), this, SLOT(on_btnDebugClicked()));
	connect(ui.btn_setHSV, SIGNAL(clicked()), this, SLOT(on_btnSetHSVClicked()));
	connect(ui.btn_setDataSheet, SIGNAL(clicked()), this, SLOT(onSetDataSheetClicked()));

	// 下位机操作按钮
	//connect(ui.btn_reset, SIGNAL(clicked()), this, SLOT(onbtnResetClicked()));
	connect(ui.btn_reset, &QPushButton::clicked, this, [this]() {
        serialPort->write(QString("#9P0T000\r\n").toLatin1());
		});
	//connect(ui.btn_stop, SIGNAL(clicked()), this, SLOT(onbtnStopClicked()));
	connect(ui.btn_stop, &QPushButton::clicked, this, [this]() {
		serialPort->write(QString("#8P0T000\n\r").toLatin1());
		});
	connect(ui.btn_handsOpen, &QPushButton::clicked, this, [this]() {
		serialPort->write(QString("#2P7T000\r\n").toLatin1());
		});
    connect(ui.btn_handsClose, &QPushButton::clicked, this, [this]() {
		serialPort->write(QString("#2P6T000\r\n").toLatin1());
		});

	// 动作按钮
	connect(ui.btn_recog, SIGNAL(clicked()), this, SLOT(on_btnRecogClicked()));
	connect(ui.btn_sendSingle, SIGNAL(clicked()), this, SLOT(on_btnSendSingleClicked()));
	connect(ui.btn_sendAll, SIGNAL(clicked()), this, SLOT(slot_sendOperationSerial()));

	// 可选择项
	connect(ui.inputCheckBox, SIGNAL(clicked()), this, SLOT(slotInputStateChange()));
	connect(ui.reuseCheckBox, SIGNAL(clicked()), this, SLOT(slotReuseStateChange()));

	// 消息框类
	connect(ui.btn_clearMessage, &QPushButton::clicked, ui.plainTextEdit_SerialRX, &QPlainTextEdit::clear);

	//COM口操作相关
	connect(ui.btn_portOpen_close, SIGNAL(clicked()), this, SLOT(on_btnPortOpenClicked()));
	connect(ui.btn_portRefresh, SIGNAL(clicked()), this, SLOT(on_btnPortRefreshClicked()));
	connect(ui.btn_portSend, SIGNAL(clicked()), this, SLOT(on_btnPortSendClicked()));
	connect(ui.comboBox_baudRate, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_baudRateChanged()));
	connect(ui.btn_stratagyConfirm, SIGNAL(clicked()), this, SLOT(on_btnStrategyConfirm()));
	ui.comboBox_coms->clear();
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
		ui.comboBox_coms->addItem(info.portName());
	}
	//connect(ui.comboBox_coms, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_portInfoChanged(QString)));
	
	// 初始化摄像头
	InitCameraEvents();

	// 初始化复原记录
	LoadRestoreRecordsFromFile();
	//on_btnCameraClicked();

	//初始化串口类对象
	serialPort = new QSerialPort();

	serialPort->setBaudRate(QSerialPort::Baud9600);
	serialPort->setDataBits(QSerialPort::Data8);
	serialPort->setParity(QSerialPort::NoParity);
	serialPort->setStopBits(QSerialPort::OneStop);
	serialPort->setFlowControl(QSerialPort::NoFlowControl);

	connect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::ReadOperationFromPort);

}

CubeExplorerWithQt::~CubeExplorerWithQt() {

	SaveRestoreRecordsToFile();

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

void CubeExplorerWithQt::InitCameraEvents()
{
    // 定义相机名称和对应的UI组件映射
	cameraCombos = {
		ui.comboBox_cameraFR,
		ui.comboBox_cameraU,
		ui.comboBox_cameraBL,
		ui.comboBox_cameraD
	};
	cameraViews = {
		ui.graView_cameraFR,
		ui.graView_cameraU,
		ui.graView_cameraBL,
		ui.graView_cameraD
	};
	sceneNameToIndex = {
		{"FR", SCENE_FR_ID},
		{"U", SCENE_U_ID},
		{"BL", SCENE_BL_ID},
		{"D", SCENE_D_ID}
	};

    // 为每个显示视图绑定对应的信号槽和初始化
	for (int i = 0; i < cameraCombos.size(); i++) {
		// 绑定下拉框信号槽
        QComboBox* comboBox = cameraCombos[i];
        connect(comboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_cameraInfoChanged(QString)));
		
		// 绑定图形视图信号槽
		QGraphicsView* graView = cameraViews[i];
		connect(graView, SIGNAL(mouseReleased(QRect)), this, SLOT(slot_mouseReleasedInCameraViews(QRect)));

		// 新建场景
		QGraphicsScene* scene = new QGraphicsScene(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2, SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT);
		graView->setScene(scene);
		graView->setCursor(Qt::CrossCursor);
		graView->setDragMode(QGraphicsView::RubberBandDrag);
        cameraScenes.append(scene);

		// 新建视频项
		QGraphicsVideoItem* videoItem = new QGraphicsVideoItem;
		videoItem->setSize(QSize(SCENE_VIEW_WIDTH, SCENE_VIEW_HEIGHT));
		videoItem->setPos(-SCENE_VIEW_WIDTH / 2, -SCENE_VIEW_HEIGHT / 2);
		scene->addItem(videoItem);
        cameraVideoItems.append(videoItem);
    }

    // 菜单动作绑定
    connect(ui.actSetBlock, SIGNAL(triggered()), this, SLOT(slot_menuSetRecTriggered()));
    connect(ui.actShowHSV, SIGNAL(triggered()), this, SLOT(slot_menuShowHSVTriggered()));
}


void CubeExplorerWithQt::onbtnCamSwitchClicked() {
	if (isCameraOpen) {
		isCameraOpen = false;
		for (int i = 0; i < cameras.size(); i++) {
			cameras[i]->stop();
			delete cameras[i];
			delete cameraCaptures[i];
		}
        ui.btn_camSwitch->setText(QStringLiteral("打开摄像头"));
		return;
	}
	ui.btn_camSwitch->setText(QStringLiteral("关闭摄像头"));
	isCameraOpen = true;

	//刷新可用摄像头信息（打开摄像头开关）
	//list_cameraInfo.clear();
	cameras.clear();
	cameraCaptures.clear();
	//list_pSnap.clear();

	for (QComboBox* comboBox : cameraCombos) {
        comboBox->clear();
        comboBox->addItem(QString::number(-1));
	}

	QCameraViewfinderSettings set; // 设置摄像头刷新率30hz, 分辨率1920*1080
	set.setMaximumFrameRate(30);
	set.setMinimumFrameRate(30);
	set.setResolution(1920, 1080);

	int i = 0;
	foreach(QCameraInfo info, QCameraInfo::availableCameras()) {
		//list_cameraInfo.append(info);

		//构建camera对象并存放在cameras中
		QCamera* camera_t = new QCamera(info);
		cameras.append(camera_t);
		
		//并构建对应于当前摄像头的capture对象，并将capture对象添加到captures中
		QCameraImageCapture* capture_t = new QCameraImageCapture(camera_t);
		capture_t->setCaptureDestination(QCameraImageCapture::CaptureToBuffer);
		connect(capture_t, &QCameraImageCapture::imageCaptured, this, &CubeExplorerWithQt::slot_imageCaptured);
		cameraCaptures.append(capture_t);

		captureToSceneIndex[capture_t] = i;

		//list_pSnap.append(snap_t);

		for (QComboBox* comboBox : cameraCombos) {
			comboBox->addItem(QString::number(i));
		}i++;
	}

	// comboBox 从1开始，0是-1无效
	for (int i = 0; i < cameras.size(); i++) {
		cameraCombos[i]->setCurrentIndex(i+1);
		cameras[i]->setViewfinder(cameraVideoItems[i]);
	}
	//cameras[0]->setViewfinder(videoItems["FR"]);
}

// 捕获并存储相机文件
void CubeExplorerWithQt::CaptureImage() { 
	nImgSaved = 0;
	for (QCameraImageCapture* cameraCapture : cameraCaptures)
		cameraCapture->capture();
	//cameraCaptures[1]->capture();
	//cameraCaptures[2]->capture();
	//cameraCaptures[3]->capture();

	/*list_pCapture[map_pic_cameraIndex["FR"]]->capture(curPath + "/pic_cam/cam_" + "FR");
	list_pCapture[map_pic_cameraIndex["U"]]->capture(curPath + "/pic_cam/cam_" + "U");
	list_pCapture[map_pic_cameraIndex["D"]]->capture(curPath + "/pic_cam/cam_" + "D");
	list_pCapture[map_pic_cameraIndex["BL"]]->capture(curPath + "/pic_cam/cam_" + "BL");*/
}



void CubeExplorerWithQt::ShowRecogResultOnScene(std::string strRec)
{
    // 定义一个辅助函数，用于设置每个方向的场景
    auto setupScene = [&](const QString& imagePath, QGraphicsView* graphicsView) {
        std::unique_ptr<QGraphicsScene> scene(new QGraphicsScene);
        std::unique_ptr<QImage> image(new QImage);

        if (!image->load(imagePath)) {
            qWarning() << "Failed to load image from path:" << imagePath;
            return; // 如果加载失败，直接返回，避免后续操作
        }

        QImage imageTmp = image->scaled(graphicsView->width() - 10, graphicsView->height() - 10);
        scene->addPixmap(QPixmap::fromImage(imageTmp));
        graphicsView->setScene(scene.release()); // 释放场景的所有权给图形视图
        graphicsView->show();
    };

    // 设置各个方向的场景
    setupScene("./pic_res/res_r.png", ui.graphicsView_R);
    setupScene("./pic_res/res_f.png", ui.graphicsView_F);
    setupScene("./pic_res/res_u.png", ui.graphicsView_U);
    setupScene("./pic_res/res_b.png", ui.graphicsView_B);
    setupScene("./pic_res/res_l.png", ui.graphicsView_L);
    setupScene("./pic_res/res_d.png", ui.graphicsView_D);
}

void CubeExplorerWithQt::LoadRestoreRecordsFromFile() {

	QFile file_record(QDir::currentPath() + "/Data/restoreRecord.txt");	
	file_record.open(QIODevice::ReadOnly);
	QTextStream ts(&file_record);
	QString str_record = ts.readAll();

	QList<QString> list_recordStr = str_record.split('_');			//根据 '_' 分割得到复原步数记录

	for (int i = 0; i < list_recordStr.size() - 1; i += 2) {		
		int cnt = list_recordStr[i].toInt();
		double time = list_recordStr[i + 1].toDouble();
		list_restoreRecords.push_back(RestoreRecord(cnt,time));
	}

	file_record.close();
}

void CubeExplorerWithQt::SaveRestoreRecordsToFile()
{
	QString data2write;
	for (int i = 0; i < list_restoreRecords.size(); i++) {
		data2write += QString::asprintf("%d_%f", list_restoreRecords[i].cnt, list_restoreRecords[i].time);
		if (i != list_restoreRecords.size() - 1) {
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

	if (cubeExplorerSPFA->GetVecStrSerial().empty()) {
		serialPort->write(QString("#2P0T200\r\n").toLatin1());
		serialPort->write(QString("#4P0T200\r\n").toLatin1());
		serialPort->write(QString("#7P0T200\r\n").toLatin1());
		serialPort->flush();
		return;
	}
	serialPort->write(QString::fromStdString(cubeExplorerSPFA->GetVecStrSerial()[0]).toLatin1());
	cubeExplorerSPFA->GetVecStrSerial().erase(cubeExplorerSPFA->GetVecStrSerial().begin());
}

//--------------------------------------------------------------------------------------
//右边栏操作按钮响应槽函数
void CubeExplorerWithQt::on_btnTightOrLooseClicked() {
	//操作两只机械手进行微松开和夹紧，用以装配和取下魔方
	serialPort->write(QString("#2P0T200\r\n").toLatin1());
	serialPort->write(QString("#4P0T200\r\n").toLatin1());
	cubeExplorerSPFA->hand_state = Hand_State(false, false);

	//if (cubeExplorerSPFA->hand_state.left_is_tight) {
	//	serialPort->write(QString("#2P0T200\r\n").toLatin1());
	//	serialPort->write(QString("#4P0T200\r\n").toLatin1());
	//	cubeExplorerSPFA->hand_state = Hand_State(false, false);
	//}
	//else {
	//	serialPort->write(QString("#2P1T200\r\n").toLatin1());
	//	serialPort->write(QString("#4P1T200\r\n").toLatin1());
	//	cubeExplorerSPFA->hand_state = Hand_State(true, true);
	//}
}

void CubeExplorerWithQt::on_btnResetClicked() {
	//操作夹子张开，两只手臂恢复到初始角度并张开，夹子合拢

//	serialPort->write(QString("#5P0T200\r\n").toLatin1()); // clamp_open

	serialPort->write(QString("#2P0T200\r\n").toLatin1());
	serialPort->write(QString("#4P0T200\r\n").toLatin1());
	isToRestore = false;

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
	pMyTimer->reset(), pMyTimer->start(); // 重置用时计时器
	pTimer->start(); // 开始动态显示计时器

	serialPort->write(QString("#2P1T75\r\n").toLatin1());
	serialPort->write(QString("#4P1T75\r\n").toLatin1());
	serialPort->flush();

	isToRestore = true; // 设置复原标志为真
	CaptureImage(); // 调用拍照函数，全部完成后继续执行

}
#define REALRU
void CubeExplorerWithQt::SolveAndRestore()
{
	std::string strRec = ""; char* cp; char* res;

	// 1.进行识别得到识别字符串
	if (inputFromBox) strRec = ui.plainTextEdit_portWrite->toPlainText().toStdString();
	else strRec = recognizeNew();

	/*ui.plainTextEdit_portWrite->setPlainText("test\n" + QString::number(strRec.length()) + "\n" + strRec.c_str());*/
	//strRec = "UUUUBUUUURRRRURRRRFFFFLFFFFDDDDFDDDDLLLLDLLLLBBBBRBBBB";
	//strRec = "UDUDUDUDURRRRRRRRRFFFFFFFFFDUDUDUDUDLLLLLLLLLBBBBBBBBB";

	// 2. 进行解算得到Solve6移动序列
	double st = clock(), ed;
	cp = new char[strRec.length() + 1];
	ui.plainTextEdit_SerialRX->insertPlainText(QStringLiteral(" -> 解算中...\n"));
	strcpy(cp, strRec.c_str()); res = CubeSolver(cp, NULL);

	// 3. 如果解算失败，则显示识别结果，并返回
	if (!res) {
		ShowRecogResultOnScene(strRec);
		ui.label_UI_message->setText(QStringLiteral("识别序列有误！"));
		ui.plainTextEdit_portWrite->setPlainText(QStringLiteral("识别序列有误！"));
		ui.plainTextEdit_SerialRX->insertPlainText(QStringLiteral("FETAL: 识别序列有误！\n"));
		pTimer->stop(); //停止计时器
		hasRobotStarted = false;
		if (!inputFromBox) {
			SaveCaptureMatToFile(curPath.toStdString() + "/pic_cam/cam_");
		}
		return;
	}

	// 4. 通过SPFA算法得到最短路径
	cubeExplorerSPFA->GetShortestPath(res);
	if (cubeExplorerSPFA->GetAnsOpStepNumber() > 77) {
		cubeExplorerSPFA = multiSolver->GetMultiThreadPath(strRec);
	}
	else cubeExplorerSPFA->SaveMechanicalStep();

	// 5. 通过串口通信把串口序列传递给控制机
	if (isToRestore) slot_sendOperationSerial();

	// 6. 显示识别结果和操作序列
	std::string ansOpSequence = cubeExplorerSPFA->GetAnsOpSequence();
	int steps = cubeExplorerSPFA->GetAnsOpStepNumber();
	ed = clock();
	std::string strDisplay = "";
	ShowRecogResultOnScene(strRec);
	ui.label_UI_message->setText(QStringLiteral("识别正确！"));
	ui.plainTextEdit_SerialRX->insertPlainText(QStringLiteral("SUCCESS: 识别正确！\n"));
	strDisplay += "RecogResult: " + strRec + "\r\n      Solve6: " + res;
	strDisplay += "\r\n      Solve2: " + ansOpSequence;
	strDisplay += "\r\n      AnsCostTime: " + to_string(cubeExplorerSPFA->GetAnsCostTime());
	strDisplay += (cubeExplorerSPFA->reuseFlag == true) ? " (True)" : " (False)";
	strDisplay += "\r\n      Total: " + to_string(steps) + " steps";
	strDisplay += "\r\n      Time: " + to_string(int(ed - st)) + "ms";
	
	ui.plainTextEdit_portWrite->setPlainText(QString::fromStdString(strDisplay));
	ui.label_restoreCnt->setText(QString::number(steps));

	// 7. 保存识别结果和操作序列
	if (!inputFromBox) {
		SaveCaptureMatToFile(curPath.toStdString() + "/pic_cam/cam_");
	}
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

void CubeExplorerWithQt::SaveCaptureMatToFile(string curPath)
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
		scene = cameraScenes[i];										//根据窗口顺序获取scene指针
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
	}
}

void CubeExplorerWithQt::on_btnRecogClicked() {
	isToRestore = false;

#ifdef REALRUN
	CaptureImage();
#endif // REALRUN

#ifndef REALRUN
	if (inputFromBox) SolveAndRestore();
	else if(isCameraOpen) CaptureImage();
	else QMessageBox::warning(this, "warning", QStringLiteral("请先打开摄像头"));
#endif // !REALRUN
}

void CubeExplorerWithQt::on_btnSetHSVClicked(){
	//HSV设置
	HSVThresholdDialog td(this);
	td.setWindowTitle(QStringLiteral("HSV阈值设置"));
	td.show(); td.exec();
}

void CubeExplorerWithQt::onSetDataSheetClicked()
{
	disconnect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::ReadOperationFromPort);
	ui.plainTextEdit_SerialRX->insertPlainText(QStringLiteral("数据表设置中，请稍等...\n"));
	DataSheetWidget dsw(serialPort, this);
	dsw.setWindowTitle(QStringLiteral("数据表设置"));
	dsw.show();
	dsw.exec();
	ui.plainTextEdit_SerialRX->insertPlainText(QStringLiteral("数据表设置完成\n"));
	connect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::ReadOperationFromPort);
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

	cameraCaptures[sceneNameToIndex[caller]]->capture(curPath+"/pic_cam/"+caller+"_temp_for_showHSV");
}

void CubeExplorerWithQt::slot_setRecArea(QString groupName,QRect rect,int faceID, int blockID)
{
	ui.label_UI_message->setText(QStringLiteral("set Rect for") + groupName + faceID + blockID);
	SamRec rec2set = { (rect.x() - 5) * 2,(rect.x() + rect.width() - 5) * 2,(rect.y() - 5) * 2,(rect.y() + rect.height() - 5) * 2 };
	setSampleRec(groupName, rec2set, faceID, blockID);		//设置采样矩阵

}

void CubeExplorerWithQt::slot_imageCaptured(int id, const QImage& image)
{
	QCameraImageCapture* senderCapture = qobject_cast<QCameraImageCapture*>(sender());
	int sceneIndex = captureToSceneIndex[senderCapture]; // 获取当前摄像头对应的场景index

	//将QImage转换为cv::Mat
	captureMatSet[sceneIndex] = QImageToCvMat(image.scaled(640, 360, Qt::IgnoreAspectRatio));

	sampleFromPic(captureMatSet[sceneIndex], list_picID[sceneIndex]);

	if (++nImgSaved == 4) SolveAndRestore();
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
		ui.label_portMessage->setText(QStringLiteral("已关闭串口"));
	}
	else {
		serialPort->setPortName(ui.comboBox_coms->currentText());
		if (serialPort->open(QIODevice::ReadWrite)) ui.label_portMessage->setText(QStringLiteral("打开串口成功"));
		else ui.label_portMessage->setText(QStringLiteral("打开串口失败"));
	}
}

void CubeExplorerWithQt::on_btnPortSendClicked() // 向串口发送输入框内信息
{
	if (serialPort->isOpen()) {
		serialPort->write(ui.plainTextEdit_portWrite->toPlainText().toLatin1());
		serialPort->flush();
	}
	else QMessageBox::warning(this, "Warning", QStringLiteral("串口未打开！！！"));
}

void CubeExplorerWithQt::slot_cameraInfoChanged(const QString & text)
{
	// 获取当前摄像头索引
	int camIndex = text.toInt();
	if (!(~camIndex)) {

		return;
	}

	QString str_t = sender()->objectName();

	// 获取当前场景的索引
	QString picName = QString(str_t[str_t.length() - 2])=="a" ? QString(str_t[str_t.length() - 1]):QString(str_t[str_t.length() - 2]) + QString(str_t[str_t.length() - 1]);
	int sceneIndex = sceneNameToIndex[picName];

	captureToSceneIndex[cameraCaptures[camIndex]] = sceneIndex;

	// 将当前摄像头的视图显示到当前场景内
	cameras[camIndex]->setViewfinder(cameraVideoItems[sceneIndex]);
}

void CubeExplorerWithQt::slot_timeout() {
	double second = pMyTimer->getTime();
	ui.lineEdit_second1->setText(QString::asprintf("%d", int(second)));
	ui.lineEdit_second2->setText(QString::asprintf("%d", int((second-int(second))*100)));
}

void CubeExplorerWithQt::slot_onReceiveTimeout()
{
	if (!byteTmp->isEmpty()) {
		QString decodedString = QTextCodec::codecForName("GB18030")->toUnicode(*byteTmp);
		ui.plainTextEdit_SerialRX->insertPlainText(decodedString);
		byteTmp->clear();
	}
}

void CubeExplorerWithQt::slotInputStateChange()
{
	if (inputFromBox) {
		ui.label_inputState->setText(QStringLiteral("已取消输入框输入"));
        ui.plainTextEdit_portWrite->clear();
	}
	else {
		ui.label_inputState->setText(QStringLiteral("请从输入框输入"));
		ui.plainTextEdit_portWrite->setPlainText("RRRRURRRRBBBBRBBBBDDDDFDDDDLLLLDLLLLFFFFLFFFFUUUUBUUUU");
	}
	inputFromBox^=true;
		
}

void CubeExplorerWithQt::slotReuseStateChange()
{
	cubeExplorerSPFA->reuseFlag ^= true;
	if(cubeExplorerSPFA->reuseFlag)
		ui.label_inputState->setText(QStringLiteral("已开启时间复用"));
	else ui.label_inputState->setText(QStringLiteral("已关闭时间复用"));
}

void CubeExplorerWithQt::slot_comReadyRead()
{
	QByteArray newData = serialPort->readAll();
	byteTmp->append(newData);
	timeoutTimer->start(100);
}

void CubeExplorerWithQt::ReadOperationFromPort()
{
	QByteArray comByteBuffer = serialPort->readAll();
	if (!comByteBuffer.isEmpty()) {
		//QString decodedString = QTextCodec::codecForName("UTF-8")->toUnicode(comByteBuffer);
		ui.plainTextEdit_SerialRX->insertPlainText(QString::fromLatin1(comByteBuffer));
		//ui.plainTextEdit_SerialRX->insertPlainText(QString::fromUtf8(comByteBuffer));
		ui.plainTextEdit_SerialRX->ensureCursorVisible();        // 确保光标可见
		
		if (comByteBuffer.contains("#Start")) {//
			if (hasRobotStarted || !isCameraOpen) return;
		//接收到开始按钮指令，开始复原
			hasRobotStarted = true;
			on_btnRestoreClicked();
			ui.label_UI_message->setText(QStringLiteral("串口收到开始信号"));
		}
		else if (comByteBuffer.contains("#Over")) {
			pTimer->stop(); isToRestore = false;
			//int cnt = cubeExplorer.transCnt;
			double time = double(int(pMyTimer->getTime() * 100)) / 100;
			//list_restoreRecords.push_back(RestoreRecord(cnt, time));
			ui.label_UI_message->setText(QStringLiteral("串口收到结束信号"));
			hasRobotStarted = false;
			handReleaseDalayTimer->start(500);
		}
		else if (comByteBuffer.contains("#Reset")) {
			hasRobotStarted = false;
		}
		else if (comByteBuffer.contains("#Relax")) {
			hasRobotStarted = false;
		}
	}
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
		connect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::ReadOperationFromPort);
		break;
	case 115200:
		disconnect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::ReadOperationFromPort);
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