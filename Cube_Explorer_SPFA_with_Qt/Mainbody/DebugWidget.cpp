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
	connect(ui.btn_save, SIGNAL(clicked()), this, SLOT(ClickBtnSave()));
	connect(ui.btn_open, SIGNAL(clicked()), this, SLOT(ClickBtnOpen()));
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
	connect(ui.btn_clamp_close, SIGNAL(clicked()), this, SLOT(ClickBtnClampClose()));
	connect(ui.btn_clamp_open, SIGNAL(clicked()), this, SLOT(ClickBtnClampOpen()));

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

	ifstream in;
	in.open(".\\Mechanical\\HandTurnState.txt", ios::in);
	in >> leftHandTurn >> rightHandTurn;
	in.close();
}

DebugWidget::~DebugWidget()
{
	ofstream out; out.open(".\\Mechanical\\HandTurnState.txt", ios::trunc);
	out << leftHandTurn << " " << rightHandTurn;
	out.close();
}


void DebugWidget::ClickBtnClampClose() {
//	serialPortDebug->write(QString("#5P1T200\r\n").toLatin1()); //clamp_close
}

void DebugWidget::ClickBtnClampOpen() {
//	serialPortDebug->write(QString("#5P0T200\r\n").toLatin1()); //clamp_open
}

void DebugWidget::on_btnShowLastSampleClicked() {
	LastSampleDialog lsd(this);
	lsd.setWindowTitle(QStringLiteral("Ê¶±ð²ÉÑù"));
	lsd.show();
	lsd.exec();
}

void DebugWidget::ClickBtnCreate() {
	qsrand(time(0));
	steps = "";
	st = (rand() % 40) + 60;
	int sta_L1 = 0, sta_L2 = 0, sta_R1 = 0, sta_R2 = 0;//1 ¿ªºÏ£¨1¿ª0ºÏ£© 2 Î»ÖÃ£¨1´¹Ö±0¸´Î»£©
	int flag_l1 = 0, flag_r1 = 0;//ÉÏÒ»²½Ðý×ª×¦
	int flag_l2 = 0, flag_r2 = 0;//ÉÏÒ»²½¿ªºÏ×¦
	for (int i = 1; i <= st; i++)
	{
		if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ±Õ¸´Î»£¬³õÊ¼×´Ì¬
		{
			int r = rand() % 8;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 4) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 5) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 6) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
			else if (r == 7) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ±Õ¸´Î»£¬×ó¿ªºÏºó
		{
			int r = rand() % 7;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 4) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 5) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
			else if (r == 6) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó±Õ¸´Î»£¬ÓÒ±Õ¸´Î»£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 7;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 4) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 5) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 6) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ±Õ¸´Î»£¬×óÐýºó
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 1) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 2) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 3) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
			else if (r == 4) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ±Õ¸´Î»£¬ÓÒÐýºó
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 4) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ±Õ´¹Ö±£¬×ó¿ªºÏºó
		{
			int r = rand() % 4;
		    if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 2) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
			else if (r == 3) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó±Õ¸´Î»£¬ÓÒ±Õ´¹Ö±£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 1) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 2) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 3) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ±Õ´¹Ö±£¬×óÐýºó
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 1) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 2) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 3) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
			else if (r == 4) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ±Õ´¹Ö±£¬ÓÒÐýºó
		{
			int r = rand() % 2;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 1) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ¿ª¸´Î»£¬×ó¿ªºÏºó
		{
			int r = rand() % 6;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 4) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 5) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó±Õ¸´Î»£¬ÓÒ¿ª¸´Î»£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 4) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ¿ª¸´Î»£¬×óÐýºó
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 1) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 2) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ¿ª¸´Î»£¬ÓÒÐýºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ¿ª´¹Ö±£¬×ó¿ªºÏºó
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 2) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó±Õ¸´Î»£¬ÓÒ¿ª´¹Ö±£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 2;
			if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 1 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ¿ª´¹Ö±£¬×óÐýºó
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 2) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 0 && sta_L2 == 0 && sta_R1 == 1 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ¸´Î»£¬ÓÒ¿ª´¹Ö±£¬ÓÒÐýºó
		{
			int r = rand() % 1;
			if (r == 0) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó±Õ´¹Ö±£¬ÓÒ±Õ¸´Î»£¬×ó¿ªºÏºó
		{
		int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó±Õ´¹Ö±£¬ÓÒ±Õ¸´Î»£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ´¹Ö±£¬ÓÒ±Õ¸´Î»£¬×óÐýºó
		{
			int r = rand() % 2;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 1) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ´¹Ö±£¬ÓÒ±Õ¸´Î»£¬ÓÒÐýºó
		{
			int r = rand() % 5;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 9; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 4) { Move[i] = 9; sta_R1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ¿ª
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó±Õ´¹Ö±£¬ÓÒ¿ª¸´Î»£¬×ó¿ªºÏºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó±Õ´¹Ö±£¬ÓÒ¿ª¸´Î»£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ´¹Ö±£¬ÓÒ¿ª¸´Î»£¬×óÐýºó
		{
			int r = rand() % 1;
			if (r == 0) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 0 && sta_L2 == 1 && sta_R1 == 1 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó±Õ´¹Ö±£¬ÓÒ¿ª¸´Î»£¬ÓÒÐýºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 8; sta_R1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 0; flag_r2 = 1; }//ÓÒ±Õ
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó¿ª¸´Î»£¬ÓÒ±Õ¸´Î»£¬×ó¿ªºÏºó
		{
			int r = rand() % 6;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 4) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 5) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó¿ª¸´Î»£¬ÓÒ±Õ¸´Î»£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 7;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó±Õ
			else if (r == 4) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 5) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 6) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó¿ª¸´Î»£¬ÓÒ±Õ¸´Î»£¬×óÐýºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 4; sta_L1 = 1; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
			else if (r == 1) { Move[i] = 5; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 2) { Move[i] = 6; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 3) { Move[i] = 7; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó¿ª¸´Î»£¬ÓÒ±Õ¸´Î»£¬ÓÒÐýºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 0; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 2; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óÄæ90
			else if (r == 3) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó±Õ
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó¿ª¸´Î»£¬ÓÒ±Õ´¹Ö±£¬×ó¿ªºÏºó
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 1) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 2) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó¿ª¸´Î»£¬ÓÒ±Õ´¹Ö±£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó±Õ
			else if (r == 1) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 2) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 3) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó¿ª¸´Î»£¬ÓÒ±Õ´¹Ö±£¬×óÐýºó
		{
			int r = rand() % 4;
			if (r == 0) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó±Õ
			else if (r == 1) { Move[i] = 5; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒË³90
			else if (r == 2) { Move[i] = 6; sta_R2 = 1; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒ180
			else if (r == 3) { Move[i] = 7; sta_R2 = 0; flag_l1 = 0; flag_r1 = 1; flag_l2 = 0; flag_r2 = 0; }//ÓÒÄæ90
		}
		else if (sta_L1 == 1 && sta_L2 == 0 && sta_R1 == 0 && sta_R2 == 1 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó¿ª¸´Î»£¬ÓÒ±Õ´¹Ö±£¬ÓÒÐýºó
		{
			int r = rand() % 1;
			if (r == 0) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó±Õ
		}
		else if (sta_L1 == 1 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 1 && flag_r2 == 0)//×ó¿ª´¹Ö±£¬ÓÒ±Õ¸´Î»£¬×ó¿ªºÏºó
		{
			int r = rand() % 2;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
		}
		else if (sta_L1 == 1 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 1)//×ó¿ª´¹Ö±£¬ÓÒ±Õ¸´Î»£¬ÓÒ¿ªºÏºó
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó±Õ
		}
		else if (sta_L1 == 1 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 1 && flag_r1 == 0 && flag_l2 == 0 && flag_r2 == 0)//×ó¿ª´¹Ö±£¬ÓÒ±Õ¸´Î»£¬×óÐýºó
		{
			int r = rand() % 1;
			if (r == 0) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
		}
		else if (sta_L1 == 1 && sta_L2 == 1 && sta_R1 == 0 && sta_R2 == 0 && flag_l1 == 0 && flag_r1 == 1 && flag_l2 == 0 && flag_r2 == 0)//×ó¿ª´¹Ö±£¬ÓÒ±Õ¸´Î»£¬ÓÒÐýºó
		{
			int r = rand() % 3;
			if (r == 0) { Move[i] = 0; sta_L2 = 0; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×óË³90
			else if (r == 1) { Move[i] = 1; sta_L2 = 1; flag_l1 = 1; flag_r1 = 0; flag_l2 = 0; flag_r2 = 0; }//×ó180
			else if (r == 2) { Move[i] = 3; sta_L1 = 0; flag_l1 = 0; flag_r1 = 0; flag_l2 = 1; flag_r2 = 0; }//×ó¿ª
		}
	}
	for (int i = 1; i <= st; i++)
	{
		steps += MechanicalMoveStr[Move[i]] + " ";
	}
	ui.textEdit->setPlainText(steps.c_str());
}

void DebugWidget::ClickBtnSave() {
	freopen("1.txt", "w", stdout);
	cout << st << endl;
	for (int i = 1; i <= st; i++)
	{
		cout << Move[i] << " ";
	}
	fclose(stdout);
}

void DebugWidget::ClickBtnOpen() {
	freopen("1.txt", "r", stdin);
	steps = "";
	cin >> st;
	for (int i = 1; i <= st; i++)
	{
		cin >> Move[i];
		steps += MechanicalMoveStr[Move[i]] + " ";
	}
	ui.textEdit->setPlainText(steps.c_str());
	fclose(stdin);
}

void DebugWidget::ClickBtnRun() {
	serialPortDebug->write(QString("#2P1T200\r\n").toLatin1());
	serialPortDebug->write(QString("#4P1T200\r\n").toLatin1());
	//serialPortDebug->write(QString("#5P0T200\r\n").toLatin1());
	//serialPortDebug->write(QString("#0P6T200\r\n").toLatin1());
	for (int i = 1; i <= st; i++)
	{
		if (Move[i] == 0) {
			++leftHandTurn;
			serialPortDebug->write(QString("#1P6T200\r\n").toLatin1());
		}
		if (Move[i] == 1) {
			if (leftHandTurn > 0) leftHandTurn -= 2, serialPortDebug->write(QString("#1P9T200\r\n").toLatin1());
			else leftHandTurn += 2, serialPortDebug->write(QString("#1P8T200\r\n").toLatin1());
		}
		if (Move[i] == 2) {
			--leftHandTurn;
			serialPortDebug->write(QString("#1P7T200\r\n").toLatin1());
		}
		if (Move[i] == 3) serialPortDebug->write(QString("#2P1T200\r\n").toLatin1());
		if (Move[i] == 4) serialPortDebug->write(QString("#2P0T200\r\n").toLatin1());
		if (Move[i] == 5) {
			++rightHandTurn;
			serialPortDebug->write(QString("#3P6T200\r\n").toLatin1());
		}
		if (Move[i] == 6) {
			if (rightHandTurn > 0) rightHandTurn -= 2, serialPortDebug->write(QString("#3P9T200\r\n").toLatin1());
			else rightHandTurn += 2, serialPortDebug->write(QString("#3P8T200\r\n").toLatin1());
		}
		if (Move[i] == 7) {
			--rightHandTurn;
			serialPortDebug->write(QString("#3P7T200\r\n").toLatin1());
		}
		if (Move[i] == 8) serialPortDebug->write(QString("#4P1T200\r\n").toLatin1());
		if (Move[i] == 9) serialPortDebug->write(QString("#4P0T200\r\n").toLatin1());
	}
	serialPortDebug->write(QString("#2P0T200\r\n").toLatin1());
	serialPortDebug->write(QString("#4P0T200\r\n").toLatin1());
	//serialPortDebug->write(QString("#0P7T200\r\n").toLatin1());
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
	++leftHandTurn;
	serialPortDebug->write(QString("#1P6T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftAnticlock() {
	--leftHandTurn;
	serialPortDebug->write(QString("#1P7T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftClockSmall() {
	serialPortDebug->write(QString("#1P3T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeftAnticlockSmall() {
	serialPortDebug->write(QString("#1P4T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnLeft180() {
	if(leftHandTurn>0) leftHandTurn-=2, serialPortDebug->write(QString("#1P8T200\r\n").toLatin1());
	else leftHandTurn += 2, serialPortDebug->write(QString("#1P8T200\r\n").toLatin1());
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
	++rightHandTurn;
	serialPortDebug->write(QString("#3P6T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightAnticlock() {
	--rightHandTurn;
	serialPortDebug->write(QString("#3P7T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightClockSmall() {
	serialPortDebug->write(QString("#3P3T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRightAnticlockSmall() {
	serialPortDebug->write(QString("#3P4T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRight180() {
	if (rightHandTurn > 0) rightHandTurn -= 2, serialPortDebug->write(QString("#3P8T200\r\n").toLatin1());
	else rightHandTurn += 2, serialPortDebug->write(QString("#3P8T200\r\n").toLatin1());
}

void DebugWidget::ClickBtnRight5Circle() {
	serialPortDebug->write(QString("#3P5T200\r\n").toLatin1());
}

void DebugWidget::ClickBtntest1() {
	for (int i = 1; i <= 100;i++) serialPortDebug->write(QString("#1P6T200\r\n").toLatin1());
}

void DebugWidget::ClickBtntest2() {
	for (int i = 1; i <= 100; i++) serialPortDebug->write(QString("#3P6T200\r\n").toLatin1());
}