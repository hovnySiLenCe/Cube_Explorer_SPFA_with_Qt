#include "RecogAreaDialog.h"

RecogAreaDialog::RecogAreaDialog(QString caller,QRect rect,QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	m_rec = rect;
	m_strCaller = caller;
//	ui.groupBtnBox_1->setTitle(QString(caller[0]));
//	ui.groupBtnBox_2->setTitle(QString(caller[1]));

	//设置总体布局为GridLayout
//	gridLayout = new QGridLayout;
//	this->setLayout(gridLayout);

//	gridLayout->addWidget(ui.groupBtnBox_1, 0, 0);
//	gridLayout->addWidget(ui.groupBtnBox_2, 0, 1);
//	resize(400, 200);

	//按钮槽绑定
	connect(ui.btn_block1_1, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block1_2, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block1_3, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block1_4, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block1_5, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block1_6, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block1_7, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block1_8, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block1_9, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_1, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_2, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_3, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_4, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_5, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_6, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_7, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_8, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block2_9, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_1, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_2, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_3, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_4, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_5, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_6, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_7, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_8, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block3_9, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_1, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_2, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_3, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_4, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_5, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_6, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_7, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_8, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block4_9, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_1, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_2, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_3, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_4, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_5, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_6, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_7, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_8, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block5_9, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_1, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_2, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_3, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_4, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_5, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_6, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_7, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_8, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));
	connect(ui.btn_block6_9, SIGNAL(clicked()), this, SLOT(on_btnsClicked()));

	//设置取色区域信号绑定
	connect(this, SIGNAL(signal_setRecArea(QString, QRect, int, int)), parent, SLOT(slot_setRecArea(QString, QRect, int, int)));
}

RecogAreaDialog::~RecogAreaDialog()
{
	//delete gridLayout;
}

void RecogAreaDialog::on_btnsClicked() {
	//弹出对话框询问是否确认当前选择
	QMessageBox::StandardButton btnReply;
	btnReply = QMessageBox::question(this, QStringLiteral("取色块确认"), QStringLiteral("确定将当前框选区域作为指定色块的取色区域吗？"), QMessageBox::Yes | QMessageBox::Cancel);
	//只在选择确定时进行设置操作
	if (btnReply == QMessageBox::Yes) {
		int iBlockID, iFaceID;
		//获取发送信号的按钮信息
		QPushButton* btn = qobject_cast<QPushButton*>(sender());	//sender()获取
		QString btnName = btn->objectName();
		//btnName="btn_block?_?"
		char cBlockID = btnName[btnName.length() - 1].unicode();
		char cFaceID = btnName[btnName.length() - 3].unicode();
		//根据按钮ObjectName后三位确定选定的魔方面号和块号
		iFaceID = cFaceID - '0';
		iBlockID = cBlockID - '0';
		
		emit signal_setRecArea(m_strCaller, m_rec, iFaceID, iBlockID);
		this->reject();
	}
}
