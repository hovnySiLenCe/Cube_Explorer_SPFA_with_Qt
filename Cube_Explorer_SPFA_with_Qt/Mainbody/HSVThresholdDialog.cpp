#include "HSVThresholdDialog.h"

HSVThresholdDialog::HSVThresholdDialog(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);

	QMap<QString, QMap<QString, HSV>>& map_hsv = getHSVMap();	//获取并复制一份hsv map数据
	map_hsv_t = map_hsv;										//	

	ui.radioBtn_f->setChecked(true);							//默认显示F面的HSV阈值数据
	showHSVData(curFace);										//
	
	//操作按钮槽函数绑定
	connect(ui.btn_apply, SIGNAL(clicked()), this, SLOT(on_btnApplyClicked()));
	connect(ui.btn_reset, SIGNAL(clicked()), this, SLOT(on_btnResetClicked()));
	connect(ui.btn_cancel, SIGNAL(clicked()), this, SLOT(on_btnCancelClicked()));

	//面选择按钮槽函数绑定
	connect(ui.radioBtn_f, SIGNAL(toggled(bool)), this, SLOT(on_faceRadioToggled(bool)));
	connect(ui.radioBtn_r, SIGNAL(toggled(bool)), this, SLOT(on_faceRadioToggled(bool)));
	connect(ui.radioBtn_u, SIGNAL(toggled(bool)), this, SLOT(on_faceRadioToggled(bool)));
	connect(ui.radioBtn_b, SIGNAL(toggled(bool)), this, SLOT(on_faceRadioToggled(bool)));
	connect(ui.radioBtn_l, SIGNAL(toggled(bool)), this, SLOT(on_faceRadioToggled(bool)));
	connect(ui.radioBtn_d, SIGNAL(toggled(bool)), this, SLOT(on_faceRadioToggled(bool)));

	//数据框槽函数绑定
	connect(ui.spin_redH_h_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redH_h_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redH_s_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redH_s_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redH_v_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redH_v_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redL_h_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redL_h_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redL_s_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redL_s_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redL_v_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_redL_v_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_orange_h_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_orange_h_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_orange_s_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_orange_s_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_orange_v_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_orange_v_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_blue_h_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_blue_h_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_blue_s_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_blue_s_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_blue_v_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_blue_v_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_green_h_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_green_h_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_green_s_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_green_s_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_green_v_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_green_v_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_yellow_h_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_yellow_h_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_yellow_s_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_yellow_s_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_yellow_v_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_yellow_v_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_white_h_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_white_h_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_white_s_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_white_s_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_white_v_min, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
	connect(ui.spin_white_v_max, SIGNAL(valueChanged(int)), this, SLOT(slot_spinValueChanged(int)));
}

HSVThresholdDialog::~HSVThresholdDialog()
{
}

void HSVThresholdDialog::showHSVData(QString face)
{

	QMap<QString, HSV> &map_color = map_hsv_t[face];

	ui.spin_redL_h_min->setValue(map_color["red_l"].iLowH);
	ui.spin_redL_h_max->setValue(map_color["red_l"].iHighH);
	ui.spin_redL_s_min->setValue(map_color["red_l"].iLowS);
	ui.spin_redL_s_max->setValue(map_color["red_l"].iHighS);
	ui.spin_redL_v_min->setValue(map_color["red_l"].iLowV);
	ui.spin_redL_v_max->setValue(map_color["red_l"].iHighV);
	ui.spin_redH_h_min->setValue(map_color["red_h"].iLowH);
	ui.spin_redH_h_max->setValue(map_color["red_h"].iHighH);
	ui.spin_redH_s_min->setValue(map_color["red_h"].iLowS);
	ui.spin_redH_s_max->setValue(map_color["red_h"].iHighS);
	ui.spin_redH_v_min->setValue(map_color["red_h"].iLowV);
	ui.spin_redH_v_max->setValue(map_color["red_h"].iHighV);

	ui.spin_orange_h_min->setValue(map_color["orange"].iLowH);
	ui.spin_orange_h_max->setValue(map_color["orange"].iHighH);
	ui.spin_orange_s_min->setValue(map_color["orange"].iLowS);
	ui.spin_orange_s_max->setValue(map_color["orange"].iHighS);
	ui.spin_orange_v_min->setValue(map_color["orange"].iLowV);
	ui.spin_orange_v_max->setValue(map_color["orange"].iHighV);

	ui.spin_blue_h_min->setValue(map_color["blue"].iLowH);
	ui.spin_blue_h_max->setValue(map_color["blue"].iHighH);
	ui.spin_blue_s_min->setValue(map_color["blue"].iLowS);
	ui.spin_blue_s_max->setValue(map_color["blue"].iHighS);
	ui.spin_blue_v_min->setValue(map_color["blue"].iLowV);
	ui.spin_blue_v_max->setValue(map_color["blue"].iHighV);

	ui.spin_green_h_min->setValue(map_color["green"].iLowH);
	ui.spin_green_h_max->setValue(map_color["green"].iHighH);
	ui.spin_green_s_min->setValue(map_color["green"].iLowS);
	ui.spin_green_s_max->setValue(map_color["green"].iHighS);
	ui.spin_green_v_min->setValue(map_color["green"].iLowV);
	ui.spin_green_v_max->setValue(map_color["green"].iHighV);

	ui.spin_yellow_h_min->setValue(map_color["yellow"].iLowH);
	ui.spin_yellow_h_max->setValue(map_color["yellow"].iHighH);
	ui.spin_yellow_s_min->setValue(map_color["yellow"].iLowS);
	ui.spin_yellow_s_max->setValue(map_color["yellow"].iHighS);
	ui.spin_yellow_v_min->setValue(map_color["yellow"].iLowV);
	ui.spin_yellow_v_max->setValue(map_color["yellow"].iHighV);

	ui.spin_white_h_min->setValue(map_color["white"].iLowH);
	ui.spin_white_h_max->setValue(map_color["white"].iHighH);
	ui.spin_white_s_min->setValue(map_color["white"].iLowS);
	ui.spin_white_s_max->setValue(map_color["white"].iHighS);
	ui.spin_white_v_min->setValue(map_color["white"].iLowV);
	ui.spin_white_v_max->setValue(map_color["white"].iHighV);
}

void HSVThresholdDialog::on_btnResetClicked()
{
	QMap<QString, QMap<QString, HSV>>& map_hsv = getHSVMap();	//获取未被修改的原始数据以重置当前使用的临时HSV数据
	map_hsv_t = map_hsv;										//
	showHSVData(curFace);										//展示数据改动
}

void HSVThresholdDialog::on_btnApplyClicked()
{
	QMap<QString, QMap<QString, HSV>>& map_hsv = getHSVMap();	//将当前使用的临时HSV数据赋值给与文件交互的HSV数据map
	map_hsv = map_hsv_t;										//
	saveHSVData();												//调用CubeRecognizer.cpp提供的函数保存HSV数据到本地文件
}

void HSVThresholdDialog::on_btnCancelClicked()
{
	this->reject();
}

void HSVThresholdDialog::on_faceRadioToggled(bool checked)
{
	if (!checked) return;
	QChar face = sender()->property("tag_face").toChar();
	curFace = face;
	showHSVData(curFace);
}

void HSVThresholdDialog::slot_spinValueChanged(int value)
{
	QString str_color = sender()->property("tag_color").toString();	//获取sender的三个动态属性来判断修改哪个值被修改了
	QChar c_hsv = sender()->property("tag_hsv").toChar();			//
	bool isMin = sender()->property("min").toBool();				//

	HSV &hsv = map_hsv_t[curFace][str_color];						//获取对应颜色的HSV引用

	//根据tag_hsv和tag_min两个动态属性判断应该对应HSV结构中的修改哪一个值
	if (isMin) switch (c_hsv.toLatin1()) {						
	case 'h':hsv.iLowH = value;
		break;
	case 's':hsv.iLowS = value;
		break;
	case 'v':hsv.iLowV = value;
		break;
	}
	else switch (c_hsv.toLatin1()) {
	case 'h':hsv.iHighH = value;
		break;
	case 's':hsv.iHighS = value;
		break;
	case 'v':hsv.iHighV = value;
		break;
	}

}
