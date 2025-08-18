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
	//multiSolver->InitSolver();

	ui.setupUi(this);

	curPath = QDir::currentPath();		//获取当前工作路径

	// 串口延时接收计时器
	byteTmp = new QByteArray(); // 用于接收串口信息
	timeoutTimer = new QTimer(this);
	timeoutTimer->setSingleShot(true);
	connect(timeoutTimer, &QTimer::timeout, this, &CubeExplorerWithQt::WaitForPortReadTimeout);

	// 延时松手计时器
	handReleaseDalayTimer = new QTimer(this);
	handReleaseDalayTimer->setSingleShot(true);
	connect(handReleaseDalayTimer, &QTimer::timeout, this, [this]() {
		ui.record_total_time->setValue(timer_stopWatch->getTime());
		ui.record_restore_time->setText(QString::asprintf("%.2f", timer_stopWatch->getTime()));
		serialPort->write(QString("#2P0T200\r\n").toLatin1());
		serialPort->write(QString("#4P0T200\r\n").toLatin1());
		ui.txt_LogDisplay->append(QStringLiteral("[INFO] Solver: 执行完毕"));
		isToRestore = false;
	});

	/*connect(ui.btn_tightOrLoose, SIGNAL(clicked()), this, SLOT(on_btnTightOrLooseClicked()));*/
	
	// 计时器组件
	InitTimerComponent();

	// 开始复原按钮
	connect(ui.btn_restore, SIGNAL(clicked()), this, SLOT(on_btnRestoreClicked()));

	// 普通操作按钮
	connect(ui.btn_camSwitch, SIGNAL(clicked()), this, SLOT(onbtnCamSwitchClicked()));
	SetHighlightButtom(ui.btn_camSwitch);

	connect(ui.btn_showSamRecs, SIGNAL(clicked()), this, SLOT(on_btnShowSamRecsClicked()));
	SetHighlightButtom(ui.btn_showSamRecs);

	connect(ui.btn_debug, SIGNAL(clicked()), this, SLOT(on_btnDebugClicked()));
	connect(ui.btn_showSampleResult, SIGNAL(clicked()), this, SLOT(on_btnShowSampleResultClicked()));
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
	connect(ui.btn_clearMessage, &QPushButton::clicked, ui.txt_LogDisplay, &QTextBrowser::clear);

	//COM口操作相关
	connect(ui.btn_portOpen_close, SIGNAL(clicked()), this, SLOT(on_btnPortOpenClicked()));
	SetHighlightButtom(ui.btn_portOpen_close);

	connect(ui.btn_portRefresh, SIGNAL(clicked()), this, SLOT(on_btnPortRefreshClicked()));
	connect(ui.btn_portSend, SIGNAL(clicked()), this, SLOT(on_btnPortSendClicked()));
	connect(ui.comboBox_baudRate, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_baudRateChanged()));
	//connect(ui.btn_stratagyConfirm, SIGNAL(clicked()), this, SLOT(on_btnStrategyConfirm()));
	ui.comboBox_coms->clear();
	foreach(QSerialPortInfo info, QSerialPortInfo::availablePorts()) {
		ui.comboBox_coms->addItem(info.portName());
	}
	ui.comboBox_coms->setCurrentIndex(1);
	connect(ui.comboBox_coms, SIGNAL(currentIndexChanged(QString)), this, SLOT(slot_portInfoChanged()));
	
	// 初始化摄像头
	InitCameraEvents();

	// 复原记录相关操作
	LoadRestoreRecordsFromCSV();
	//LoadRestoreRecordsFromFile();
	ui.recordTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.recordTable, &QTableWidget::customContextMenuRequested, this, [this](QPoint pos) {
		QTableWidgetItem* item = ui.recordTable->itemAt(pos);
		if (item) {
			QMenu menu;
			menu.addAction(QStringLiteral("删除"), [this, item]() {
				int row = item->row();
				ui.recordTable->removeRow(row);
				});
			menu.exec(ui.recordTable->mapToGlobal(pos));
			}
		});
	// 保存记录按钮
	connect(ui.btn_addRecord, &QPushButton::clicked, this, [this]() {
		bool ok;
		QString description = QInputDialog::getText(this,
			QStringLiteral("输入描述"),
			QStringLiteral("请输入描述信息："),
			QLineEdit::Normal,
			QString(),
			&ok);
		if (ok) {
			AppendRestoreRecordsToTable(description);
			ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 保存记录成功"));
		}
		else {
			ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 保存记录取消"));
		}
		});
	connect(ui.record_total_time, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), this, [=](double totalTime) {
		double restoreTime = ui.record_restore_time->text().toDouble();
		ui.record_manual_time->setText(QString::number(totalTime - restoreTime));
		});
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

	//SaveRestoreRecordsToFile();
	SaveRestoreRecordsToCSV();

	if (serialPort->isOpen()) {
		serialPort->close();
		delete serialPort;
	}

	delete timer_stopWatch;
	delete timer_displayRefresh;
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

	ui.btn_camSwitch->setEnabled(false);

	if (isCameraOpen) {
		SetHighlightButtom(ui.btn_camSwitch);
		isCameraOpen = false;
		for (int i = 0; i < cameras.size(); i++) {
			disconnect(cameraCaptures[i], &QCameraImageCapture::imageCaptured, this, &CubeExplorerWithQt::slot_imageCaptured);
			delete cameraCaptures[i];
			cameras[i]->stop();
			delete cameras[i];
		}
		for (QComboBox* comboBox : cameraCombos)
			comboBox->clear();
        ui.btn_camSwitch->setText(QStringLiteral("打开摄像头"));
		ui.btn_camSwitch->setEnabled(true);
		return;
	}
	else {
		SetCommonStyButtom(ui.btn_camSwitch);
	}
	ui.btn_camSwitch->setText(QStringLiteral("关闭摄像头"));
	isCameraOpen = true;

	//刷新可用摄像头信息（打开摄像头开关）
	//list_cameraInfo.clear();
	cameras.clear();
	cameraCaptures.clear();
	captureToSceneIndex.clear();
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
		QCamera* camera_t = new QCamera(info, this);
		camera_t->setViewfinderSettings(set);
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
		cameras[i]->start();
	}
	//cameras[0]->setViewfinder(videoItems["FR"]);

	ui.btn_camSwitch->setEnabled(true);
}

// 捕获并存储相机文件
void CubeExplorerWithQt::CaptureImage() { 
	nImgSaved = 0;
	for (QCameraImageCapture* cameraCapture : cameraCaptures)
		cameraCapture->capture();
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

void CubeExplorerWithQt::LoadRestoreRecordsFromCSV(QString filename)
{
	QFile file(filename);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, "Error", QStringLiteral("无法打开文件"));
		return;
	}

	QTextStream in(&file);
	in.setCodec("UTF-8");  // 处理中文编码

	// 读取表头
	QString headerLine = in.readLine();
	QStringList headers = headerLine.split(',');  // 假设分隔符为逗号

	// 加载数据前关闭刷新
	ui.recordTable->setUpdatesEnabled(false);

	ui.recordTable->setColumnCount(headers.size());
	ui.recordTable->setHorizontalHeaderLabels(headers);

	// 清空旧数据
	ui.recordTable->setRowCount(0);

	// 逐行读取数据
	int row = 0;
	while (!in.atEnd()) {
		QString line = in.readLine().trimmed();
		if (line.isEmpty()) continue;  // 跳过空行

		QStringList fields = line.split(',');
		if (fields.size() != headers.size()) {
			QMessageBox::warning(this, "Warning", QStringLiteral("第 ") + QString::number(row + 1) + QStringLiteral(" 行列数不匹配"));
			continue;
		}

		// 插入新行并填充数据
		ui.recordTable->insertRow(row);
		for (int col = 0; col < fields.size(); ++col) {
			QTableWidgetItem* item = new QTableWidgetItem(fields[col]);
			item->setTextAlignment(Qt::AlignCenter);
			ui.recordTable->setItem(row, col, item);
		}
		row++;
	}

	file.close();

	// 加载完成后恢复刷新
	ui.recordTable->setUpdatesEnabled(true);

	// 自动调整列宽（基于内容+表头）
	ui.recordTable->resizeColumnsToContents();

	// 设置列宽调整策略（允许手动调整+自动最小宽度）
	ui.recordTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
	ui.recordTable->horizontalHeader()->setMinimumSectionSize(50);

	// 启用滚动条边缘弹性效果（Qt5.7+）
	ui.recordTable->setProperty("kineticScrolling", true);
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

void CubeExplorerWithQt::SaveRestoreRecordsToCSV(QString filename)
{

	QFile file(filename);
	bool isNewFile = !file.exists();
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QMessageBox::critical(this, "Error", QStringLiteral("保存RestoreRecords时出错，无法打开文件"));
		return;
	}

	QTextStream out(&file);
	out.setCodec("UTF-8");  // 保持与读取时一致的编码

	if (isNewFile) {
		out.setGenerateByteOrderMark(true);
		QStringList headers = { "Timestamp", "Strategy", "Steps", "Total Time",
			"Restore Time", "Manual Time", "Description",
			"Recognize Result", "Kociemba Result", "Solve2", "Calc Time", "Answer Cost" };
		out << headers.join(",") << "\n";
	}

	// 写入表头
	QStringList headers;
	for (int col = 0; col < ui.recordTable->columnCount(); ++col) {
		headers << FormatCSVField(ui.recordTable->horizontalHeaderItem(col)->text());
	}
	out << headers.join(',') << "\n";

	// 写入数据行
	for (int row = 0; row < ui.recordTable->rowCount(); ++row) {
		QStringList rowData;
		for (int col = 0; col < ui.recordTable->columnCount(); ++col) {
			QTableWidgetItem* item = ui.recordTable->item(row, col);
			rowData << FormatCSVField(item ? item->text() : "");
		}
		out << rowData.join(',') << "\n";
	}

	file.close();
	
}

void CubeExplorerWithQt::AppendRestoreRecordsToTable(QString str_description)
{
	QVariantList record = {
		QDateTime::currentDateTime(),
		ui.record_strategy_name->currentText(),
        ui.record_steps->text(),
		ui.record_total_time->value(),
		ui.record_restore_time->text(),
		ui.record_manual_time->text(),
		str_description,
		ui.txt_RecogResult->text(),
		ui.txt_KociembaResult->text(),
		ui.txt_AnsOpSequence->toPlainText(),
		ui.txt_CalcTime->text(),
		ui.txt_AnswerCost->text()
	};

	QTableWidget* table = ui.recordTable;

	const int newRow = table->rowCount();
	table->insertRow(newRow);
    for (int i = 0; i < record.size(); ++i) {
		QTableWidgetItem* item = new QTableWidgetItem(FormatCSVField(record[i]));
		item->setTextAlignment(Qt::AlignCenter);
		table->setItem(newRow, i, item);
	}
	table->scrollToBottom();       // 滚动到底部
}

QString CubeExplorerWithQt::FormatCSVField(const QVariant& value)
{
	QString str;

	// 根据不同类型进行格式化
	switch (value.type()) {
	case QMetaType::QDateTime:
		str = value.toDateTime().toString("yyyy-MM-dd HH:mm:ss");
		break;
	case QMetaType::Double:
		str = QString::number(value.toDouble(), 'f', 3); // 保留6位小数
		break;
	default:
		str = value.toString();
		break;
	}

	// 处理字段中的特殊字符
	if (str.contains(',') || str.contains('"') || str.contains('\n')) {
		str.replace(",", " ");
		str.replace("\n", " ");
		str.replace("\"", "\"\"");
		//return "\"" + str + "\"";
	}
	return str;
}

void CubeExplorerWithQt::slot_sendOperationSerial() {
	if (!serialPort->isOpen()) {
		QMessageBox::warning(this, "Warning", QStringLiteral("又忘记开串口了！！"));
		return;
	}
	timer_stopWatch->reset();
	timer_stopWatch->start();
	timer_displayRefresh->start();
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
	serialPort->write(QString("#4P0T200\r\n").toLatin1());
	serialPort->write(QString("#7P0T200\r\n").toLatin1());
	serialPort->flush();
}

//优化解算方法
void CubeExplorerWithQt::on_btnSendSingleClicked() { // 单步执行解算

	if (cubeExplorerSPFA->GetVecStrSerial().empty()) {
		serialPort->write(QString("#2P0T200\r\n").toLatin1());
		serialPort->write(QString("#4P0T200\r\n").toLatin1());
		serialPort->flush();
		QMessageBox::information(this, "Notice", QStringLiteral("已全部执行完成"));
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

void CubeExplorerWithQt::InitTimerComponent()
{
	timer_displayRefresh = new QTimer(this);			//构造并绑定计时器槽函数
	timer_displayRefresh->setInterval(15); //设置计时器间隔，用于刷新显示复原用时
	
	timer_stopWatch = new MyTimer();
	connect(timer_displayRefresh, &QTimer::timeout, this, &CubeExplorerWithQt::TimerDisplayRefresh);

	static QIcon icon_start("./qtWindows/source/start.png");
	static QIcon icon_stop("./qtWindows/source/stop.png");
	static QIcon icon_reset("./qtWindows/source/reset.png");
    ui.btn_timerStop->setIcon(icon_start);
	ui.btn_timerReset->setIcon(icon_reset);
	connect(ui.btn_timerStop, &QPushButton::clicked, [this]() {
		if (timer_stopWatch->isRunning()) {
			timer_stopWatch->stop();
			timer_displayRefresh->stop();
			ui.btn_timerStop->setIcon(icon_start);
		}
		else {
			timer_stopWatch->start();
			timer_displayRefresh->start();
			ui.btn_timerStop->setIcon(icon_stop);
		}
		});
	connect(ui.btn_timerReset, &QPushButton::clicked, [this]() {
		timer_stopWatch->reset();
		timer_displayRefresh->stop();
		TimerDisplayRefresh();
		});
}

void CubeExplorerWithQt::SetHighlightButtom(QPushButton* buttom)
{
	buttom->setStyleSheet(
		"QPushButton {"
		"   background-color: #4CAF50;"  // 绿色
		"   color: white;"
		"   border-radius: 5px;"
		"}"
		"QPushButton:hover {"
		"   background-color: #45a049;"  // 深绿色（悬停效果）
		"}"
	);
}

void CubeExplorerWithQt::SetCommonStyButtom(QPushButton* buttom)
{
	buttom->setStyleSheet(
		"QPushButton {"
		//"   background-color: #f0f0f0;"  // 浅灰
		"   background-color: #FFFFFF"
		"   color: black;"
		"   border-radius: 5px;"
		"}"
		"QPushButton:hover {"
		"   background-color: #e0e0e0;"  // 深灰色（悬停效果）
		"}"
	);
}

void CubeExplorerWithQt::CleanSolverResultDisplay()
{
	ui.txt_RecogResult->clear();
	ui.txt_KociembaResult->clear();
	ui.txt_AnsOpSequence->clear();
	ui.txt_CalcTime->clear();
	ui.txt_TotalSteps->clear();
	ui.txt_AnswerCost->clear();
}

void CubeExplorerWithQt::SetSolverResultDisplay()
{
	//ansOpSequence = cubeExplorerSPFA->GetAnsOpSequenceFormat();
	int steps = cubeExplorerSPFA->GetAnsOpStepNumber();

	// 在当前结果区显示计算结果
	ui.txt_RecogResult->setText(QString::fromStdString(recogResult));
	//ui.txt_KociembaResult->setText(QString::fromStdString(kociembaResult));
	ui.txt_AnsOpSequence->setText(QString::fromStdString(cubeExplorerSPFA->GetAnsOpSequenceFormat()));
	ui.txt_AnswerCost->setText(QString::number(cubeExplorerSPFA->GetAnsCostTime()) + ((cubeExplorerSPFA->reuseFlag == true) ? " (True)" : " (False)"));
	ui.txt_TotalSteps->setText(QString::number(steps) + "steps");
	ui.txt_CalcTime->setText(QString::number(ed - st) + "ms");

	// 在计时器区域显示计算结果
	ui.label_restoreCnt->setText(QString::number(steps));
	ui.txt_LogDisplay->append(QStringLiteral("[SUCCESS] Solver: 识别正确！"));

	// 添加记录信息
	ui.record_steps->setText(QString::number(steps));
	ui.record_total_time->setValue(0);
	ui.record_restore_time->setText("0");
	ui.record_manual_time->setText("0");
}

void CubeExplorerWithQt::on_btnRestoreClicked() {
	timer_stopWatch->reset(), timer_stopWatch->start(); // 重置用时计时器
	timer_displayRefresh->start(); // 开始动态显示计时器

	serialPort->write(QString("#2P1T75\r\n").toLatin1());
	serialPort->write(QString("#4P1T75\r\n").toLatin1());
	serialPort->flush();

	isToRestore = true; // 设置复原标志为真
	CaptureImage(); // 调用拍照函数，全部完成后继续执行

}
#define REALRU
void CubeExplorerWithQt::SolveAndRestore()
{
	//char* cp;

	// 1.进行识别得到识别字符串
	if (inputFromBox) recogResult = ui.txt_RecogResult->text().toStdString();
	else recogResult = recognizeNew();
	//qDebug() << "Recog Result:" << QString::fromStdString(recogResult);
	CleanSolverResultDisplay();

	// 2. 进行解算得到Solve6移动序列
	st = ed = clock();
	//cp = new char[recogResult.length() + 1];
	ui.txt_LogDisplay->append(QStringLiteral("[INFO] Solver: 正在计算"));
	//strcpy(cp, recogResult.c_str());
	//qDebug() << "On Clicked Time:" << st;
	//kociembaResult = CubeSolver(cp, NULL);

	// 3. 如果解算失败，则显示识别结果，并返回
	
	ed = clock();
	//qDebug() << "Finish Procession Time:" << ed << " Duration: " << ed - st;
	// 4. 通过SPFA算法得到最短路径
	cubeExplorerSPFA = multiSolver->GetMultiThreadPath(recogResult);
	if (cubeExplorerSPFA->ansTime == -1) {
		ShowRecogResultOnScene(recogResult);

		ui.txt_RecogResult->setText(QStringLiteral("识别序列有误！"));
		ui.txt_LogDisplay->append(QStringLiteral("[FATAL] Solver: 识别序列有误！"));
		ui.label_restoreCnt->setText("##");

		timer_displayRefresh->stop(); //停止计时器
		hasRobotStarted = false;
		if (!inputFromBox) {
			SaveCaptureMatToFile(curPath.toStdString() + "/pic_cam/cam_");
		}
		return;
	}

	cubeExplorerSPFA->SaveMechanicalStep();
	ed = clock();
	//qDebug() << "Finish SaveStep Time:" << ed << " Duration: " << ed - st;
	//cubeExplorerSPFA->GetShortestPath(res);
	//if (cubeExplorerSPFA->GetAnsOpStepNumber() > 77) {
	//	cubeExplorerSPFA = multiSolver->GetMultiThreadPath(strRec);
	//}
	//else cubeExplorerSPFA->SaveMechanicalStep();

	// 5. 通过串口通信把串口序列传递给控制机
	if (isToRestore) slot_sendOperationSerial();

	// 6. 显示识别结果和操作序列
	ed = clock();
	ShowRecogResultOnScene(recogResult);
	SetSolverResultDisplay();

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
		SetHighlightButtom(ui.btn_showSamRecs);
		return;
	}
	SetCommonStyButtom(ui.btn_showSamRecs);
	QMap<QString, vector<SamRec>> &map_pic_id_samRec = getSamRecMap();	//获取采样框数据map
	vector<SamRec> map_id_samRec;
	QGraphicsScene* scene;
	QRect rect;
	QGraphicsRectItem *item;

	QPen pen;	//自定义画笔进行item的绘画
	pen.setWidth(1);	//
	pen.setColor("#4CAF50");	//

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

void CubeExplorerWithQt::on_btnShowSampleResultClicked(){
	/* 旧版显示采样数据 */
	/*LastSampleDialog lsd(this);
	lsd.setWindowTitle(QStringLiteral("识别采样"));
	lsd.show();
	lsd.exec();*/
	/* 新版显示采样数据 */
	HSVCorrectionDialog hcd(this);
	hcd.setWindowTitle(QStringLiteral("识别采样"));
    hcd.show();
    hcd.exec();
}

void CubeExplorerWithQt::onSetDataSheetClicked()
{
	disconnect(serialPort, &QSerialPort::readyRead, this, &CubeExplorerWithQt::ReadOperationFromPort);
	ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 数据表设置中，请稍等..."));
	DataSheetWidget dsw(serialPort, this);
	dsw.setWindowTitle(QStringLiteral("数据表设置"));
	dsw.show();
	dsw.exec();
	ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 数据表设置完成"));
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

	on_btnShowSamRecsClicked();
	on_btnShowSamRecsClicked();
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
	ui.txt_LogDisplay->setText(QStringLiteral("set Rect for") + groupName + faceID + blockID);
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

void CubeExplorerWithQt::slot_portInfoChanged()
{
	serialPort->close();
	ui.btn_portOpen_close->setText(QStringLiteral("打开串口"));
	SetHighlightButtom(ui.btn_portOpen_close);
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
		ui.btn_portOpen_close->setText(QStringLiteral("打开串口"));
		ui.txt_LogDisplay->append(QStringLiteral("[SUCCESS] 成功关闭串口") + ui.comboBox_coms->currentText());
		SetHighlightButtom(ui.btn_portOpen_close);
	}
	else {
		serialPort->setPortName(ui.comboBox_coms->currentText());
		if (serialPort->open(QIODevice::ReadWrite)) {
			ui.txt_LogDisplay->append(QStringLiteral("[SUCCESS] 成功打开串口") + ui.comboBox_coms->currentText());
			ui.btn_portOpen_close->setText(QStringLiteral("关闭串口"));
			SetCommonStyButtom(ui.btn_portOpen_close);
		}
		else QMessageBox::critical(this, "Error", QStringLiteral("打开串口失败"));
	}
}

void CubeExplorerWithQt::on_btnPortSendClicked() // 向串口发送输入框内信息
{
	if (serialPort->isOpen()) {
		serialPort->write(ui.txt_messageSend->text().toLatin1());
		serialPort->flush();
	}
	else QMessageBox::warning(this, "Warning", QStringLiteral("串口未打开！！！"));
}

void CubeExplorerWithQt::slot_cameraInfoChanged(const QString & text)
{
	if (!isCameraOpen) return;
	// 获取当前摄像头索引
	int camIndex = text.toInt();

	QString str_t = sender()->objectName();
	
	// 获取当前场景的索引
	QString picName = QString(str_t[str_t.length() - 2]) == "a" ? QString(str_t[str_t.length() - 1]) : QString(str_t[str_t.length() - 2]) + QString(str_t[str_t.length() - 1]);
	int sceneIndex = sceneNameToIndex[picName];

	// 获取显示控件的索引
	QGraphicsVideoItem* videoItem = cameraVideoItems[sceneIndex];

	if (!(~camIndex)) {
		videoItem->hide();
		return;
	}

	captureToSceneIndex[cameraCaptures[camIndex]] = sceneIndex;

	// 将当前摄像头的视图显示到当前场景内
	cameras[camIndex]->setViewfinder(videoItem);
	videoItem->show();
}

void CubeExplorerWithQt::TimerDisplayRefresh() {
	double second = timer_stopWatch->getTime();
	ui.lineEdit_second1->setText(QString::asprintf("%d", int(second)));
	ui.lineEdit_second2->setText(QString::asprintf("%02d", int((second-int(second))*100)));
}

void CubeExplorerWithQt::WaitForPortReadTimeout()
{
	if (!byteTmp->isEmpty()) {
		QString decodedString = QTextCodec::codecForName("GB18030")->toUnicode(*byteTmp);
		ui.txt_LogDisplay->insertPlainText(decodedString);
		byteTmp->clear();
	}
}

void CubeExplorerWithQt::slotInputStateChange()
{
	if (inputFromBox) {
		ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 取消输入框输入"));
		CleanSolverResultDisplay();
	}
	else {
		ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 从输入框输入"));
		CleanSolverResultDisplay();
		ui.txt_RecogResult->setText("RRRRURRRRBBBBRBBBBDDDDFDDDDLLLLDLLLLFFFFLFFFFUUUUBUUUU");
	}
	inputFromBox^=true;
		
}

void CubeExplorerWithQt::slotReuseStateChange()
{
	cubeExplorerSPFA->reuseFlag ^= true;
	if(cubeExplorerSPFA->reuseFlag)
		ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 已开启时间复用"));
	else ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 已关闭时间复用"));
}

void CubeExplorerWithQt::slot_comReadyRead()
{
	QByteArray newData = serialPort->readAll();
	byteTmp->append(newData);
	timeoutTimer->start(100);
}

void CubeExplorerWithQt::ReadOperationFromPort()
{
	static QByteArray comByteBuffer = "";
	comByteBuffer.append(serialPort->readAll());
	//ui.txt_LogDisplay->append(QString::number(comByteBuffer.size()));
	if (comByteBuffer.contains("\n")) {
		//QString decodedString = QTextCodec::codecForName("UTF-8")->toUnicode(comByteBuffer);
		//QTextCursor cursor(ui.txt_LogDisplay->document());
		//cursor.movePosition(QTextCursor::End);
		
		int pos = comByteBuffer.lastIndexOf("\n");
		ui.txt_LogDisplay->append(QString::fromLatin1(comByteBuffer.left(pos-1)));

		if (comByteBuffer.contains("#Sta")) {//
			if (hasRobotStarted || !isCameraOpen) {
				ui.txt_LogDisplay->append(hasRobotStarted?QStringLiteral("[INFO] Solver: 已经开始复原"):QStringLiteral("[FATAL] Solver: 摄像头未打开"));
				comByteBuffer.clear();
				return;
			}
			//接收到开始按钮指令，开始复原
			hasRobotStarted = true;
			on_btnRestoreClicked();
			ui.txt_LogDisplay->append(QStringLiteral("[INFO] Solver: 开始复原"));
			//ui.label_UI_message->setText(QStringLiteral("串口收到开始信号"));
		}
		if (comByteBuffer.contains("#O")) {
			timer_stopWatch->stop(), timer_displayRefresh->stop();
			TimerDisplayRefresh();
			isToRestore = hasRobotStarted = false;
			handReleaseDalayTimer->start(500);
			//int cnt = cubeExplorer.transCnt;
			//double time = double(int(timer_stopWatch->getTime() * 100)) / 100;
			//list_restoreRecords.push_back(RestoreRecord(cnt, time));
			//ui.label_UI_message->setText(QStringLiteral("串口收到结束信号"));
		}
		if (comByteBuffer.contains("#Reset")) {
			hasRobotStarted = false;
		}
		if (comByteBuffer.contains("#Relax")) {
			timer_stopWatch->stop();
			ui.record_total_time->setValue(99.999);
			ui.record_restore_time->setText("99.99");
			//ui.txt_LogDisplay->append(QStringLiteral("[INFO] User: 紧急停止"));
			hasRobotStarted = false;
		}
		comByteBuffer.remove(0, pos+1);
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
	//ui.plainTextEdit_portWrite->setPlainText(strDis);
	ui.txt_LogDisplay->clear();
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

//void CubeExplorerWithQt::on_btnStrategyConfirm()
//{
//	if (!serialPort->isOpen()) {
//		ui.label_portMessage->setText(QStringLiteral("串口未打开！！！"));
//		return;
//	}
//
//	string strategy = ui.comboBox_strategy->currentText().toStdString();
//
//	switch (strategy[0]) {
//	case 's':
//		serialPort->write("\r\n");
//		serialPort->write("set_rotate_rise_speed_180 130000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed 130000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed_close 150000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed_close_1 150000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_speed_180 140000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_90 140000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_close 140000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_close_1 140000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		/*serialPort->write(QString("set_continu_rotate_speed 200000\r\n").toLatin1());
//		serialPort->write(QString("set_Positioning_speed 5000\r\n").toLatin1());*/
//		serialPort->write(QString("set_rotate_over_delay 10\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_open_over_delay 10\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_close_over_delay 10\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		break;
//	case 'm':
//		serialPort->write("\r\n");
//		serialPort->write("set_rotate_rise_speed_180 150000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed 150000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed_close 160000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed_close_1 160000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_speed_180 200000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_90 200000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_close 200000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_close_1 200000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		/*serialPort->write(QString("set_continu_rotate_speed 200000\r\n").toLatin1());
//		serialPort->write(QString("set_Positioning_speed 5000\r\n").toLatin1());*/
//		serialPort->write(QString("set_rotate_over_delay 10\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_open_over_delay 15\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_close_over_delay 10\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		break;
//	case 'f':
//		serialPort->write("\r\n");
//		serialPort->write("set_rotate_rise_speed_180 160000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed 160000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed_close 160000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_rise_speed_close_1 160000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write("set_rotate_speed_180 200000\r\n");
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_90 220000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_close 220000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_close_1 220000\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		/*serialPort->write(QString("set_continu_rotate_speed 200000\r\n").toLatin1());
//		serialPort->write(QString("set_Positioning_speed 5000\r\n").toLatin1());*/
//		serialPort->write(QString("set_rotate_over_delay 5\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_open_over_delay 20\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_close_over_delay 5\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		break;
//	case 'u':
//		serialPort->write("\r\n");
//		serialPort->write(QString("set_rotate_rise_speed_180 150000\r\n").toLatin1());  		//  set_rotate_rise_speed_180 - 设置手爪翻转180旋转升降速度      带魔方整体翻转180度的加速度
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_rise_speed 150000\r\n").toLatin1());		        //	set_rotate_rise_speed - 设置手爪翻转90旋转升降速度           带魔方整体翻转90度的加速度
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_rise_speed_close 190000\r\n").toLatin1());		//	set_rotate_rise_speed_close - 设置对面手爪闭合时旋转升降速度  空转 + 拧魔方翻转90 + 180度的加速度
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_rise_speed_close_1 190000\r\n").toLatin1());		//	set_rotate_rise_speed_close_1 - 设置对面手爪闭合时连续旋转升降速度 ？？？连续转90 / 180度的第二次
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_180 180000\r\n").toLatin1());				//	set_rotate_speed_180 - 设置手爪翻转180旋转速度  带魔方整体翻转180度的最大速度
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_90 180000\r\n").toLatin1());				//	set_rotate_speed_90 - 设置手爪翻转90旋转速度 带魔方整体翻转90度的最大速度
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_close 190000\r\n").toLatin1());				//	set_rotate_speed_close - 设置对面手爪闭合时手爪旋转速度 空转 + 拧魔方翻转90 + 180度的最大速度
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_rotate_speed_close_1 190000\r\n").toLatin1());			//	set_rotate_speed_close_1 - 设置对面手爪闭合时手爪连续旋转速度   ？？？连续转90 / 180度的第二次的最大速度
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		/*serialPort->write(QString("set_continu_rotate_speed 200000\r\n").toLatin1());
//		serialPort->write(QString("set_Positioning_speed 5000\r\n").toLatin1());*/
//		serialPort->write(QString("set_rotate_over_delay 5\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_open_over_delay 8\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		serialPort->write(QString("set_close_over_delay 8\r\n").toLatin1());
//		serialPort->waitForBytesWritten();
//		QThread::msleep(100);
//		break;
//
//	}
//	serialPort->write("print_para\r\n");
//	serialPort->flush();
//}