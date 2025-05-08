#include "DebugWidget.h"

DebugWidget::DebugWidget(QSerialPort* serialPort, QWidget *parent)
	: serialPortDebug(serialPort), QDialog(parent), ui(Ui::DebugWidget())
{
	ui.setupUi(this);

	connect(ui.btn_showLastSample, SIGNAL(clicked()), this, SLOT(on_btnShowLastSampleClicked()));
	connect(ui.btn_left_close, SIGNAL(clicked()), this, SLOT(ClickBtnLeftClose()));
	connect(ui.btn_left_close_small, SIGNAL(clicked()), this, SLOT(ClickBtnLeftCloseSmall()));
	connect(ui.btn_left_close_half, SIGNAL(clicked()), this, SLOT(ClickBtnLeftHalfClose()));
	connect(ui.btn_left_open, SIGNAL(clicked()), this, SLOT(ClickBtnLeftOpen()));
	connect(ui.btn_left_open_small, SIGNAL(clicked()), this, SLOT(ClickBtnLeftOpenSmall()));
	connect(ui.btn_left_open_all, SIGNAL(clicked()), this, SLOT(ClickBtnLeftOpenAll()));
	connect(ui.btn_create, SIGNAL(clicked()), this, SLOT(ClickBtnCreate()));
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(ClickBtnExportSequence()));
	connect(ui.btn_open, SIGNAL(clicked()), this, SLOT(ClickBtnImportSequence()));
	connect(ui.btn_run, SIGNAL(clicked()), this, SLOT(ClickBtnRun()));
	connect(ui.btn_left_to90, SIGNAL(clicked()), this, SLOT(ClickBtnLeftTo90()));
	connect(ui.btn_left_to180, SIGNAL(clicked()), this, SLOT(ClickBtnLeftTo180()));
	connect(ui.btn_left_clockwise, SIGNAL(clicked()), this, SLOT(ClickBtnLeftClock()));
	connect(ui.btn_left_anticlockwise, SIGNAL(clicked()), this, SLOT(ClickBtnLeftAnticlock()));
	connect(ui.btn_left_clockwise_small, SIGNAL(clicked()), this, SLOT(ClickBtnLeftClockSmall()));
	connect(ui.btn_left_anticlockwise_small, SIGNAL(clicked()), this, SLOT(ClickBtnLeftAnticlockSmall()));
	connect(ui.btn_left_180, SIGNAL(clicked()), this, SLOT(ClickBtnLeft180()));
	connect(ui.btn_left_5circle, SIGNAL(clicked()), this, SLOT(ClickBtnLeft5Circle()));
	connect(ui.btn_right_close, SIGNAL(clicked()), this, SLOT(ClickBtnRightClose()));
	connect(ui.btn_right_close_small, SIGNAL(clicked()), this, SLOT(ClickBtnRightCloseSmall()));
	connect(ui.btn_right_close_half, SIGNAL(clicked()), this, SLOT(ClickBtnRightHalfClose()));
	connect(ui.btn_right_open, SIGNAL(clicked()), this, SLOT(ClickBtnRightOpen()));
	connect(ui.btn_right_open_small, SIGNAL(clicked()), this, SLOT(ClickBtnRightOpenSmall()));
	connect(ui.btn_right_open_all, SIGNAL(clicked()), this, SLOT(ClickBtnRightOpenAll()));
	connect(ui.btn_right_to90, SIGNAL(clicked()), this, SLOT(ClickBtnRightTo90()));
	connect(ui.btn_right_to180, SIGNAL(clicked()), this, SLOT(ClickBtnRightTo180()));
	connect(ui.btn_right_clockwise, SIGNAL(clicked()), this, SLOT(ClickBtnRightClock()));
	connect(ui.btn_right_anticlockwise, SIGNAL(clicked()), this, SLOT(ClickBtnRightAnticlock()));
	connect(ui.btn_right_clockwise_small, SIGNAL(clicked()), this, SLOT(ClickBtnRightClockSmall()));
	connect(ui.btn_right_anticlockwise_small, SIGNAL(clicked()), this, SLOT(ClickBtnRightAnticlockSmall()));
	connect(ui.btn_right_180, SIGNAL(clicked()), this, SLOT(ClickBtnRight180()));
	connect(ui.btn_right_5circle, SIGNAL(clicked()), this, SLOT(ClickBtnRight5Circle()));

	connect(ui.btn_test1, SIGNAL(clicked()), this, SLOT(ClickBtntest1()));
	connect(ui.btn_test2, SIGNAL(clicked()), this, SLOT(ClickBtntest2()));

	QGraphicsScene* scene_FR = new QGraphicsScene;
	QGraphicsScene* scene_U = new QGraphicsScene;
	QGraphicsScene* scene_BL = new QGraphicsScene;
	QGraphicsScene* scene_D = new QGraphicsScene;

	QImage* image = new QImage();
	QImage imageTmp;

	image->load(QString("./pic_cam/cam_FR.jpg"));
	imageTmp = image->scaled(ui.graphicsView_FR->width() - 10, ui.graphicsView_FR->height() - 10);
	scene_FR->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_FR->setScene(scene_FR);
	ui.graphicsView_FR->show();

	image->load(QString("./pic_cam/cam_U.jpg"));
	imageTmp = image->scaled(ui.graphicsView_U->width() - 10, ui.graphicsView_U->height() - 10);
	scene_U->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_U->setScene(scene_U);
	ui.graphicsView_U->show();

	image->load(QString("./pic_cam/cam_BL.jpg"));
	imageTmp = image->scaled(ui.graphicsView_BL->width() - 10, ui.graphicsView_BL->height() - 10);
	scene_BL->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_BL->setScene(scene_BL);
	ui.graphicsView_BL->show();

	image->load(QString("./pic_cam/cam_D.jpg"));
	imageTmp = image->scaled(ui.graphicsView_D->width() - 10, ui.graphicsView_D->height() - 10);
	scene_D->addPixmap(QPixmap::fromImage(imageTmp));
	ui.graphicsView_D->setScene(scene_D);
	ui.graphicsView_D->show();

	st = 0;
	seqs = "";
}

DebugWidget::~DebugWidget()
{
	// 写入文件
	QFile file("./Data/GeneratedOperationSequence.txt");
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this, "Error", QStringLiteral("无法创建文件：") + file.errorString());
		return;
	}

	QTextStream stream(&file);
	stream << st << "\n";
	for (int i = 1; i <= st; i++) stream << Move[i] << " ";
	stream.flush();
	file.close();
}

void DebugWidget::on_btnShowLastSampleClicked() {
	LastSampleDialog lsd(this);
	lsd.setWindowTitle(QStringLiteral("识别采样"));
	lsd.show();
	lsd.exec();
}

void DebugWidget::ClickBtnCreate() {
	qsrand(time(0));
	st = (rand() % 40) + 60;
	int sta_L1 = 0, sta_L2 = 0, sta_R1 = 0, sta_R2 = 0;//1 开合（1开0合） 2 位置（1垂直0复位）
	int flag_l1 = 0, flag_r1 = 0;//上一步旋转爪
	int flag_l2 = 0, flag_r2 = 0;//上一步开合爪
	for (int i = 1; i <= st; i++)
	{
		if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右闭复位，初始状态
		{
			int r = rand() % 8;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 4) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 5) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 6) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
			else if (r == 7) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左闭复位，右闭复位，左开合后
		{
			int r = rand() % 7;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 4) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 5) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
			else if (r == 6) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左闭复位，右闭复位，右开合后
		{
			int r = rand() % 7;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 4) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 5) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 6) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右闭复位，左旋后
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 1) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 2) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 3) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
			else if (r == 4) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右闭复位，右旋后
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 4) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左闭复位，右闭垂直，左开合后
		{
			int r = rand() % 4;
		    if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 2) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
			else if (r == 3) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左闭复位，右闭垂直，右开合后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 1) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 2) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 3) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右闭垂直，左旋后
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 1) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 2) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 3) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
			else if (r == 4) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右闭垂直，右旋后
		{
			int r = rand() % 2;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 1) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左闭复位，右开复位，左开合后
		{
			int r = rand() % 6;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 4) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 5) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左闭复位，右开复位，右开合后
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 4) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右开复位，左旋后
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 1) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 2) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右开复位，右旋后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左闭复位，右开垂直，左开合后
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 2) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左闭复位，右开垂直，右开合后
		{
			int r = rand() % 2;
			if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 1 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右开垂直，左旋后
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 2) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左闭复位，右开垂直，右旋后
		{
			int r = rand() % 1;
			if (r == 0) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左闭垂直，右闭复位，左开合后
		{
		int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左闭垂直，右闭复位，右开合后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左闭垂直，右闭复位，左旋后
		{
			int r = rand() % 2;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 1) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左闭垂直，右闭复位，右旋后
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 9; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 4) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右开
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左闭垂直，右开复位，左开合后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左闭垂直，右开复位，右开合后
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左闭垂直，右开复位，左旋后
		{
			int r = rand() % 1;
			if (r == 0) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左闭垂直，右开复位，右旋后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//右闭
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左开复位，右闭复位，左开合后
		{
			int r = rand() % 6;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 4) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 5) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左开复位，右闭复位，右开合后
		{
			int r = rand() % 7;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左闭
			else if (r == 4) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 5) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 6) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左开复位，右闭复位，左旋后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
			else if (r == 1) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 2) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 3) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左开复位，右闭复位，右旋后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左逆90
			else if (r == 3) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左闭
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左开复位，右闭垂直，左开合后
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 2) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左开复位，右闭垂直，右开合后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左闭
			else if (r == 1) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 2) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 3) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左开复位，右闭垂直，左旋后
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左闭
			else if (r == 1) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右顺90
			else if (r == 2) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右180
			else if (r == 3) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//右逆90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左开复位，右闭垂直，右旋后
		{
			int r = rand() % 1;
			if (r == 0) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左闭
		}
		else if (sta_L1 == 1 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//左开垂直，右闭复位，左开合后
		{
			int r = rand() % 2;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
		}
		else if (sta_L1 == 1 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//左开垂直，右闭复位，右开合后
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左闭
		}
		else if (sta_L1 == 1 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//左开垂直，右闭复位，左旋后
		{
			int r = rand() % 1;
			if (r == 0) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
		}
		else if (sta_L1 == 1 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//左开垂直，右闭复位，右旋后
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左顺90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//左180
			else if (r == 2) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//左开
		}
	}

	seqs = "";
	for (int i = 1; i <= st; i++) seqs += MechanicalMoveStr[Move[i]] + ((i % 20) ? " " : "\n");
	ui.seqs_length->display(st);
	ui.textEdit->setPlainText(seqs);

}

void DebugWidget::ClickBtnExportSequence() {
    // 获取保存路径
	QString filePath = QFileDialog::getSaveFileName(
		this,
		QStringLiteral("导出文件"),
		QDir::cleanPath(QDir::currentPath() + "/Data") + "/GeneratedOperationSequence.txt",
		QStringLiteral("TXT 文件 (*.txt);;所有文件 (*)")
	);

	if (filePath.isEmpty()) return; // 用户取消操作

	// 写入文件
	QFile file(filePath);
	if (!file.open(QIODevice::WriteOnly)) {
		QMessageBox::critical(this, "Error", QStringLiteral("无法创建文件：") + file.errorString());
		return;
	}

	QTextStream stream(&file);
	stream << st << "\n";
	for(int i = 1; i <= st; i++) stream << Move[i] << " ";
	stream.flush();
	file.close();

	QMessageBox::information(this, "Success", QStringLiteral("文件导出成功！"));
}

void DebugWidget::ClickBtnImportSequence() {

	// 获取打开路径
	QString filePath = QFileDialog::getOpenFileName(
		this,
		QStringLiteral("导入文件"),
		QDir::currentPath() + "/Data/GeneratedOperationSequence.txt",
		QStringLiteral("TXT 文件 (*.txt);;所有文件 (*)")
	);

	if (filePath.isEmpty()) return; // 用户取消操作

	// 写入文件
	QFile file(filePath);
	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::critical(this, "Error", QStringLiteral("无法打开文件：") + file.errorString());
		return;
	}
	QTextStream stream(&file);
	stream >> st; seqs = "";
	for (int i = 1; i <= st; i++) {
		stream >> Move[i];
		seqs += MechanicalMoveStr[Move[i]] + ((i % 20)?" ":"\n");
	}
	file.close();
	ui.seqs_length->display(st);
	ui.textEdit->setPlainText(seqs);
}

void DebugWidget::ClickBtnRun() {
	// 预定义常用指令
	const QByteArray cmdList[] = {
		"#1P6T200\r\n",
		"#1P8T200\r\n",
		"#1P7T200\r\n",
		"#2P1T200\r\n",
		"#2P0T200\r\n",
		"#3P6T200\r\n",
		"#3P8T200\r\n",
		"#3P7T200\r\n",
		"#4P1T200\r\n",
		"#4P0T200\r\n"
	};

	serialPortDebug->write("#2P1T200\r\n", 9);
	serialPortDebug->write("#4P1T200\r\n", 9);

	// 发送Move相关指令
	for (int i = 1; i <= st; i++) {
		if (Move[i] >= 0 && Move[i] <= 9) {
			serialPortDebug->write(cmdList[Move[i]]);
		} else {
			// 处理非法值，可以选择记录日志或采取默认动作
			qWarning() << "Invalid Move value at index" << i << ": " << Move[i];
		}
	}

	serialPortDebug->write("#2P0T200\r\n", 9);
	serialPortDebug->write("#4P0T200\r\n", 9);
}

void DebugWidget::ClickBtnLeftClose() {
	serialPortDebug->write(QString("#2P1T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftCloseSmall() {
	serialPortDebug->write(QString("#2P4T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftHalfClose() {
	serialPortDebug->write(QString("#2P2T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftOpen() {
	serialPortDebug->write(QString("#2P0T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftOpenSmall() {
	serialPortDebug->write(QString("#2P3T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftOpenAll() {
	serialPortDebug->write(QString("#2P5T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftTo90() {
	serialPortDebug->write(QString("#1P90T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftTo180() {
	serialPortDebug->write(QString("#1P180T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftClock() {
	serialPortDebug->write(QString("#1P6T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftAnticlock() {
	serialPortDebug->write(QString("#1P7T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftClockSmall() {
	serialPortDebug->write(QString("#1P3T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftAnticlockSmall() {
	serialPortDebug->write(QString("#1P4T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeft180() {
	serialPortDebug->write(QString("#1P8T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeft5Circle() {
	serialPortDebug->write(QString("#1P5T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightClose() {
	serialPortDebug->write(QString("#4P1T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightCloseSmall() {
	serialPortDebug->write(QString("#4P4T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightHalfClose() {
	serialPortDebug->write(QString("#4P2T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightOpen() {
	serialPortDebug->write(QString("#4P0T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightOpenSmall() {
	serialPortDebug->write(QString("#4P3T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightOpenAll() {
	serialPortDebug->write(QString("#4P5T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightTo90() {
	serialPortDebug->write(QString("#3P90T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightTo180() {
	serialPortDebug->write(QString("#3P180T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightClock() {
	serialPortDebug->write(QString("#3P6T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightAnticlock() {
	serialPortDebug->write(QString("#3P7T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightClockSmall() {
	serialPortDebug->write(QString("#3P3T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightAnticlockSmall() {
	serialPortDebug->write(QString("#3P4T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRight180() {
	serialPortDebug->write(QString("#3P8T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRight5Circle() {
	serialPortDebug->write(QString("#3P5T200\r\n").toLatin1());
}

void DebugWidget::ClickBtntest1() {
	for (int i = 1; i <= 10;i++) serialPortDebug->write(QString("#1P6T200\r\n").toLatin1());
}

void DebugWidget::ClickBtntest2() {
	for (int i = 1; i <= 10; i++) serialPortDebug->write(QString("#3P6T200\r\n").toLatin1());
}