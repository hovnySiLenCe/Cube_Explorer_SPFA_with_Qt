#include "HSVCorrectionDialog.h"

HSVCorrectionDialog::HSVCorrectionDialog(QWidget *parent)
	: QDialog(parent), ui(new Ui::HSVCorrectionDialog), m_correData(getCorreDataMap())
{
	// ui初始化
	ui->setupUi(this);

	// 获取采样框mat
	mat_map = getLastRecogMatMap();

	// 初始化数据显示寄存
	m_curData = m_correData;
	m_realData = m_correData;

	// 添加结果布局
	for each (QString strFace in m_listFace)
	{
		addButtonToResultGrid(strFace);
		//addLineEditToRealGrid(strFace);
		//addLineEditToCorrectionGrid(strFace);
	}
	
	// 初始化基准值索引
	standardIndex = make_pair("u", 4);
	displayStandardHighlight();

	// 显示真实数据结果
	calcRealHSV();
    displayRealHSV();

	// 绑定按钮
	connect(ui->btn_confModi, &QPushButton::clicked, this, &HSVCorrectionDialog::onbtnConfModiClicked);
	connect(ui->btn_readPara, &QPushButton::clicked, this, &HSVCorrectionDialog::onbtnReadParaClicked);
	connect(ui->btn_autoCorrect, &QPushButton::clicked, this, &HSVCorrectionDialog::onbtnAutoCorrectClicked);
	
	// 基准值选择操作
	connect(ui->cBox_faceChoice, &QComboBox::currentTextChanged, this, &HSVCorrectionDialog::displayStandardHighlight);
	connect(ui->cBox_idChoice, &QComboBox::currentTextChanged, this, &HSVCorrectionDialog::displayStandardHighlight);

	// 保存参数版本操作
	connect(ui->btn_saveStrategy, &QPushButton::clicked, this, &HSVCorrectionDialog::onbtnSaveVersionClicked);
	connect(ui->btn_delStrategy, &QPushButton::clicked, this, &HSVCorrectionDialog::onbtnDelVersionClicked);
	connect(ui->list_strategy, &QListWidget::itemDoubleClicked, this, &HSVCorrectionDialog::slot_OpenSelectedVersion);
	connect(ui->list_strategy, &QListWidget::itemClicked, this, &HSVCorrectionDialog::slot_RefreshVersionName);
	connect(ui->list_strategy, &QListWidget::itemChanged, this, &HSVCorrectionDialog::onListItemChanged);

	// 文件导入导出操作
	connect(ui->btn_exportFile, &QPushButton::clicked, this, &HSVCorrectionDialog::onbtnExportFileClicked);
	connect(ui->btn_importFile, &QPushButton::clicked, this, &HSVCorrectionDialog::onbtnImportFileClicked);
	LoadCorreDataFromFile("correctionDataset.json");
}

HSVCorrectionDialog::~HSVCorrectionDialog()
{

}

void HSVCorrectionDialog::addButtonToResultGrid(QString strFace)
{
	QGridLayout* curfaceLayout = this->findChild<QGridLayout*>("gLay_res"+strFace.toUpper());
	if (!curfaceLayout) {
		qWarning() << "gLay_resF" + strFace.toUpper() + "布局不存在";
		return;
	}
	clearLayout(curfaceLayout); // 2. 清空布局内容

	// 3. 添加新内容到布局
	for (int i = 0; i < 9; ++i) {
		QPushButton* btn = new QPushButton();
		btn->setObjectName(QString("btn_res%1%2").arg(strFace).arg(i));
		btn->setMinimumSize(50, 50);
		btn->setMaximumSize(50, 50);
		curfaceLayout->addWidget(btn, i / 3, i % 3); // 3x3网格
        connect(btn, &QPushButton::clicked, this, &HSVCorrectionDialog::onbtnSampleBlockClicked);
		setIconBySample(btn, strFace, i);
	}

	// 4. 更新布局
	curfaceLayout->update();
}

void HSVCorrectionDialog::addLineEditToRealGrid(QString strFace)
{
	// 1. 获取三个布局
	QGridLayout* faceLayout_H = this->findChild<QGridLayout*>("gLay_real" + strFace.toUpper() + "_H");
	QGridLayout* faceLayout_S = this->findChild<QGridLayout*>("gLay_real" + strFace.toUpper() + "_S");
	QGridLayout* faceLayout_V = this->findChild<QGridLayout*>("gLay_real" + strFace.toUpper() + "_V");

	// 检查布局是否存在
	if (!faceLayout_H || !faceLayout_S || !faceLayout_V) {
		qWarning() << "HSV布局不存在: "
			<< "gLay_real" + strFace.toUpper() + "_H/S/V";
		return;
	}

	// 清空三个布局的内容
	clearLayout(faceLayout_H);
	clearLayout(faceLayout_S);
	clearLayout(faceLayout_V);

	// 为H、S、V三个布局分别添加LineEdit
	addLineEditsToLayout(faceLayout_H, strFace, "H");
	addLineEditsToLayout(faceLayout_S, strFace, "S");
	addLineEditsToLayout(faceLayout_V, strFace, "V");
}

// 辅助函数：清空布局
void HSVCorrectionDialog::clearLayout(QGridLayout* layout)
{
	if (!layout) return;

	QLayoutItem* item;
	while ((item = layout->takeAt(0)) != nullptr) {
		if (item->widget()) {
			delete item->widget();
		}
		delete item;
	}
}

// 辅助函数：向布局添加LineEdit
void HSVCorrectionDialog::addLineEditsToLayout(QGridLayout* layout,
	const QString& face,
	const QString& hsvType)
{
	if (!layout) return;

	for (int i = 0; i < 9; ++i) {
		QLineEdit* lineEdit = new QLineEdit();

		// 设置对象名称
		lineEdit->setObjectName(
			QString("lineEdit_Correction%1%2_%3")
			.arg(face)
			.arg(i)
			.arg(hsvType));
		lineEdit->setFixedSize(43, 43);
		lineEdit->setText("50");
		lineEdit->setReadOnly(true);
		lineEdit->setAlignment(Qt::AlignCenter);

		// 添加到布局 (3x3网格)
		layout->addWidget(lineEdit, i / 3, i % 3);
	}
	// 更新布局
	layout->update();
}

void HSVCorrectionDialog::addLineEditToCorrectionGrid(QString strFace)
{

}

void HSVCorrectionDialog::setIconBySample(QPushButton* btn, QString strFace, int id)
{
	//QMap<QString, vector<cv::Mat>>& mat_map = getLastRecogMatMap(); // 获取最近一次识别结果
	//QMessageBox::warning(this, "Error", strFace + QString::number(id));
	cv::Mat mat_t;
	cv::cvtColor(mat_map[strFace][id], mat_t, cv::COLOR_BGR2RGB);

	QImage img(mat_t.data, mat_t.cols, mat_t.rows, mat_t.step, QImage::Format_RGB888);

	QSize iconSize;
	if (img.width() > img.height()) iconSize = QSize(50, float(img.height()) / img.width() * 50);
	else if (img.height() > img.width()) iconSize = QSize(float(img.width()) / img.height() * 50, 50);
	btn->setIconSize(iconSize);
	btn->setIcon(QIcon(QPixmap::fromImage(img)));
}

void HSVCorrectionDialog::readCorrectionFromLayout(QGridLayout* layout, int beginId, vector<int>& correction)
{
	for (int i = 0; i < 9; ++i) {
        QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(i / 3, i % 3)->widget());
        correction[beginId + i] = lineEdit->text().toInt();
	}
}

void HSVCorrectionDialog::displayCorrectionOnLayout(QGridLayout* layout, int beginId, vector<int>& dataArray)
{
	for (int i = 0; i < 9; i++) {
		QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(i / 3, i % 3)->widget());
        lineEdit->setText(QString::number(dataArray[i + beginId]));
	}
}

void HSVCorrectionDialog::displayStandardHighlight()
{

	// 清除原先的高亮显示
	QString strFace = standardIndex.first;
	int index = standardIndex.second;
	QGridLayout* layout = this->findChild<QGridLayout*>("gLay_real" + strFace.toUpper() + "_V");
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(index / 3, index % 3)->widget());
	QPalette pal = lineEdit->palette();
	pal.setColor(QPalette::Base, Qt::white);  // 恢复背景色
	lineEdit->setPalette(pal);
	//QMessageBox::information(this, "INFO", "请选择一个标准色块");

	// 设置新的高亮显示
	strFace = ui->cBox_faceChoice->currentText();
	index = ui->cBox_idChoice->currentText().toInt();
	layout = this->findChild<QGridLayout*>("gLay_real" + strFace.toUpper() + "_V");
	lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(index / 3, index % 3)->widget());
	pal = lineEdit->palette();
	pal.setColor(QPalette::Base, QColor(200, 196, 255));  // 设置新背景色
	lineEdit->setPalette(pal);
	standardIndex = make_pair(strFace, index);
}

void HSVCorrectionDialog::calcRealHSV()
{
	for (int faceId = 0; faceId < m_listFace.size(); faceId++) {
		for (int index = 0; index < 9; index++) {
			int sumH = 0, sumS = 0, sumV = 0;
			int cntH = 0, cntS = 0, cntV = 0;
			cv::Mat mat_hsv;
			cv::cvtColor(mat_map[m_listFace[faceId]][index], mat_hsv, cv::COLOR_BGR2HSV);
			for (int i = 0; i < mat_hsv.rows; i++) {				//遍历图片每个像素
				for (int j = 0; j < mat_hsv.cols; j++) {
					cv::Point p(j, i);
					if (mat_hsv.at<cv::Vec3b>(p)[0] >= 0 && mat_hsv.at<cv::Vec3b>(p)[0] <= 150) {
						sumH += mat_hsv.at<cv::Vec3b>(p)[0];
						cntH++;
					}
					if (mat_hsv.at<cv::Vec3b>(p)[1] >= 0 && mat_hsv.at<cv::Vec3b>(p)[1] <= 255) {
						sumS += mat_hsv.at<cv::Vec3b>(p)[1];
						cntS++;
					}
					if (mat_hsv.at<cv::Vec3b>(p)[2] >= 0 && mat_hsv.at<cv::Vec3b>(p)[2] <= 255) {
						sumV += mat_hsv.at<cv::Vec3b>(p)[2];
						cntV++;
					}
				}
			}
			m_realData["H"][faceId * 9 + index] = (double)sumH / cntH;
			m_realData["S"][faceId * 9 + index] = (double)sumS / cntS;
			m_realData["V"][faceId * 9 + index] = (double)sumV / cntV;
		}
	}
}

void HSVCorrectionDialog::displayRealHSV()
{
	for each (QString strHSV in m_listHSV)
	{
		for (int i = 0; i < m_listFace.size(); i++)
		{
			QGridLayout* layout = this->findChild<QGridLayout*>("gLay_real" + m_listFace[i].toUpper() + "_" + strHSV);
			for (int j = 0; j < 9; j++)
			{
				QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(j / 3, j % 3)->widget());
				lineEdit->setText(QString::number(m_realData[strHSV][i * 9 + j]));
			}
		}
	}
}

bool HSVCorrectionDialog::IsCurrentSetDiffFromUI()
{
	for each (QString strHSV in m_listHSV) {
		for (int i = 0; i < m_listFace.size(); i++) {
			QGridLayout* layout = this->findChild<QGridLayout*>("gLay_corre" + m_listFace[i].toUpper() + "_" + strHSV);
			for (int j = 0; j < 9; j++) {
				QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(j / 3, j % 3)->widget());
				if (m_curData[strHSV][i * 9 + j] != lineEdit->text().toInt()) {
					return true;
				}
			}
		}
	}
	return false;
}

void HSVCorrectionDialog::UpdateCurrentSetFromUI()
{
	for each (QString strHSV in m_listHSV)
	{
		for (int i = 0; i < m_listFace.size(); i++)
		{
			QGridLayout* layout = this->findChild<QGridLayout*>("gLay_corre" + m_listFace[i].toUpper() + "_" + strHSV);
			for (int j = 0; j < 9; j++)
			{
				QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(j / 3, j % 3)->widget());
				m_curData[strHSV][i * 9 + j] = lineEdit->text().toInt();
			}
		}
	}
}

void HSVCorrectionDialog::UpdateUIFromCurrentSet()
{
	for each (QString strHSV in m_listHSV)
	{
		for (int i = 0; i < m_listFace.size(); i++)
		{
			QGridLayout* layout = this->findChild<QGridLayout*>("gLay_corre" + m_listFace[i].toUpper() + "_" + strHSV);
			for (int j = 0; j < 9; j++)
			{
				QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(j / 3, j % 3)->widget());
				lineEdit->setText(QString::number(m_curData[strHSV][i * 9 + j]));
			}
		}
	}
}

void HSVCorrectionDialog::LoadCorreDataFromFile(const QString& filename) {
	QString realPath = filename;
	if (!realPath.contains(":")) realPath = "./Data/" + realPath;
	QFile file(realPath);
	m_correDataset.clear(); // 清空现有数据

	if (!file.open(QIODevice::ReadOnly)) {
		QMessageBox::warning(this, "Warning", QStringLiteral("文件打开失败！"));
		return;
	}

	// 读取并解析 JSON
	QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
	if (doc.isNull()) {
		QMessageBox::warning(this, "Warning", QStringLiteral("JSON 解析失败！"));
		return;
	}

	QJsonObject rootObj = doc.object();
	for (auto itOuter = rootObj.begin(); itOuter != rootObj.end(); ++itOuter) {
		QString outerKey = itOuter.key();
		QJsonObject innerObj = itOuter.value().toObject();

		QMap<QString, vector<int>> innerMap;

		// 遍历内层 QJsonObject
		for (auto itInner = innerObj.begin(); itInner != innerObj.end(); ++itInner) {
			QString innerKey = itInner.key();
			QJsonArray jsonArray = itInner.value().toArray();

			vector<int> vec;
			for (const QJsonValue& val : jsonArray) {
				vec.push_back(val.toInt());
			}

			innerMap[innerKey] = vec;
		}

		m_correDataset[outerKey] = innerMap;
	}

	// 从最后一项开始删除
	int itemCount = ui->list_strategy->count();
	for (int i = itemCount - 1; i >= 2; --i) {
		QListWidgetItem* item = ui->list_strategy->takeItem(i);
		delete item; // 手动释放内存
	}

	// 更新 UI（排除特定项）
	if (!m_correDataset.isEmpty()) {
		for (auto it = m_correDataset.begin(); it != m_correDataset.end(); ++it) {
			if (it.key() == QStringLiteral("默认参数") || it.key() == QStringLiteral("历史恢复数据"))
				continue;
			QListWidgetItem* item = new QListWidgetItem(it.key());
			item->setData(Qt::UserRole, it.key());
			ui->list_strategy->addItem(item);
		}
	}
}

bool HSVCorrectionDialog::SaveCorreDataToFile(const QString& filename) {
    QJsonObject rootObj;

    // 遍历 m_correDataset（外层：QString → QMap<QString, QVector<int>>）
    for (auto itOuter = m_correDataset.constBegin(); itOuter != m_correDataset.constEnd(); ++itOuter) {
        const QString &outerKey = itOuter.key();
        const QMap<QString, vector<int>> &innerMap = itOuter.value();

        QJsonObject innerObj;

        // 遍历内层 QMap（QString → QVector<int>）
        for (auto itInner = innerMap.constBegin(); itInner != innerMap.constEnd(); ++itInner) {
            const QString &innerKey = itInner.key();
            const vector<int> &vec = itInner.value();

            QJsonArray jsonArray;
            for (int num : vec) {
                jsonArray.append(num);
            }

            innerObj[innerKey] = jsonArray;
        }

        rootObj[outerKey] = innerObj;
    }

    // 确保目录存在
    QDir dir("./Data");
    if (!dir.exists()) {
        dir.mkpath(".");  // 创建目录（如果不存在）
    }
	qDebug() << "filename=" << filename;
	QString realPath = filename;
    // 写入文件
	if (!realPath.contains(":")) realPath = "./Data/" + realPath;
	qDebug() << "realPath= " << realPath;
    QFile file(realPath);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open file for writing:" << file.errorString();
        return false;
    }

    QJsonDocument doc(rootObj);
    file.write(doc.toJson(QJsonDocument::Indented));  // 使用 Indented 使 JSON 可读
    file.close();

    return true;
}

bool HSVCorrectionDialog::IsUserCancelAction(const QString message)
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, "confirm", message + QStringLiteral("，确定要执行此操作吗？"),
		QMessageBox::Yes | QMessageBox::No);

	return (reply == QMessageBox::No);
}

void HSVCorrectionDialog::onbtnConfModiClicked()
{
	for(int i = 0; i < m_listFace.size(); ++i)
	{
		for each (QString strHSV in m_listHSV) {
			QGridLayout* faceLayout = this->findChild<QGridLayout*>("gLay_corre" + m_listFace[i].toUpper() + "_" + strHSV);
			readCorrectionFromLayout(faceLayout, i * 9, m_correData[strHSV]);
		}
	}
	ui->text_logger->appendPlainText(QStringLiteral("[INFO] 确认修改成功！"));
	ui->text_logger->appendPlainText(QStringLiteral("[INFO] Correction_V:"));
	for (int i = 0; i < 54; ++i)
	{
		ui->text_logger->insertPlainText(QString::number(m_correData["V"][i]) + " ");
		if (i % 9 == 8) ui->text_logger->insertPlainText("\n");
	}
	//QMessageBox::information(this, "Info", QStringLiteral("修改成功"));
}

void HSVCorrectionDialog::onbtnAutoCorrectClicked()
{
	QString strFace = standardIndex.first;
	int index = standardIndex.second;
	QGridLayout* layout = this->findChild<QGridLayout*>("gLay_real" + strFace.toUpper() +"_V");
	QLineEdit* lineEdit = qobject_cast<QLineEdit*>(layout->itemAtPosition(index / 3, index % 3)->widget());
	
	int standValue = lineEdit->text().toInt();

	for each(QString strFace in m_listFace) {
		QGridLayout* realLayout = this->findChild<QGridLayout*>("gLay_real" + strFace.toUpper() + "_V");
		QGridLayout* correLayout = this->findChild<QGridLayout*>("gLay_corre" + strFace.toUpper() + "_V");
		
		for (int i = 0; i < 9; i++) {
			QLineEdit* realEdit = qobject_cast<QLineEdit*>(realLayout->itemAtPosition(i / 3, i % 3)->widget());
			QLineEdit* correEdit = qobject_cast<QLineEdit*>(correLayout->itemAtPosition(i / 3, i % 3)->widget());
            correEdit->setText(QString::number(standValue - realEdit->text().toInt()));
		}
        
	}
	ui->text_logger->appendPlainText(QStringLiteral("[INFO] 自动校正成功！"));
}

void HSVCorrectionDialog::onbtnReadParaClicked()
{
	for (int i = 0; i < m_listFace.size(); ++i) {
		for each (QString strHSV in m_listHSV) {
			QGridLayout* faceLayout = this->findChild<QGridLayout*>("gLay_corre" + m_listFace[i].toUpper() + "_" + strHSV.toUpper());
			displayCorrectionOnLayout(faceLayout, i * 9, m_correData[strHSV]);
 		}
	}
	ui->text_logger->appendPlainText(QStringLiteral("[INFO] 读取参数成功！"));
}

void HSVCorrectionDialog::onbtnSaveVersionClicked()
{
	QString versionName = ui->text_versionName->text().trimmed();
	if (versionName.isEmpty()) {
		QMessageBox::warning(this, "error", QStringLiteral("版本名称不能为空！"));
		return;
	}
	if (m_correDataset.contains(versionName)) {
		if (IsUserCancelAction(QStringLiteral("版本已存在")))
			return;
	}
	else {
		QListWidgetItem* item = new QListWidgetItem(versionName);
		item->setData(Qt::UserRole, versionName);
		ui->list_strategy->addItem(item);
	}
	UpdateCurrentSetFromUI();
	m_correDataset.insert(versionName, m_curData);
}

void HSVCorrectionDialog::onbtnDelVersionClicked() {
	if (ui->list_strategy->currentRow() < 2) {
		QMessageBox::warning(this, "error", QStringLiteral("系统选项无法删除"));
		return;
	}
	if (QListWidgetItem* item = ui->list_strategy->currentItem()) {
		m_correDataset.remove(item->text());
		delete ui->list_strategy->takeItem(ui->list_strategy->row(item));
	}
	else {
		QMessageBox::warning(this, "warning", QStringLiteral("请先选择要删除的项"));
	}
}

void HSVCorrectionDialog::onbtnSampleBlockClicked() {
	int index = QString(*(sender()->objectName().end() - 1)).toInt();						//得到点击发生点击事件的按钮序号
	QString strFace = QString(*(sender()->objectName().end() - 2));
	//QMessageBox::warning(this, "Error", strFace + QString::number(index));
	QMap<QString, vector<cv::Mat>>& map_face_id_lastRecogMat = getLastRecogMatMap();
	HSVDataDialog hsvDialog(map_face_id_lastRecogMat[strFace][index], this);					//显示采样框HSV可视化统计数据
	hsvDialog.setWindowTitle("HSV-" + strFace.toUpper() + QString::number(index + 1));
	hsvDialog.show();
	hsvDialog.exec();
}

void HSVCorrectionDialog::onListItemChanged(QListWidgetItem* item)
{
	// 获取修改后的文本
	QString newName = item->text().trimmed();

	// 获取该项的原始名称（存储在UserRole中）
	QString oldName = item->data(Qt::UserRole).toString();

	if (m_correDataset.contains(oldName)) {
		// 获取旧版本数据
		QMap<QString, vector<int>> corredata = m_correDataset.value(oldName);

		m_correDataset.remove(oldName);
		m_correDataset.insert(newName, corredata);
		item->setData(Qt::UserRole, newName);
	}
	else {
		qWarning() << "找不到对应的版本数据:" << oldName;
	}
}

void HSVCorrectionDialog::slot_OpenSelectedVersion(QListWidgetItem* item)
{
	if (IsCurrentSetDiffFromUI() && IsUserCancelAction(QStringLiteral("你有修改未保存"))) return;
	m_curData = m_correDataset.value(item->text());
	UpdateUIFromCurrentSet();
}

void HSVCorrectionDialog::slot_RefreshVersionName(QListWidgetItem* item)
{
	ui->text_versionName->setText(item->text());
}

void HSVCorrectionDialog::onbtnExportFileClicked()
{
	// 获取保存路径
	QString filePath = QFileDialog::getSaveFileName(
		this,
		QStringLiteral("导出文件"),
		QDir::currentPath() + "/Data/correctionDataset.json",
		QStringLiteral("JSON 文件 (*.json);;所有文件 (*)")
	);

	if (filePath.isEmpty()) return; // 用户取消操作
	qDebug() << "filePath= " << filePath;
    if (SaveCorreDataToFile(filePath))
		QMessageBox::information(this, "Success", QStringLiteral("文件导出成功！"));
    else
		QMessageBox::critical(this, "Error", QStringLiteral("无法创建文件："));
}

void HSVCorrectionDialog::onbtnImportFileClicked()
{
	// 获取打开路径
	QString filePath = QFileDialog::getOpenFileName(
		this,
		QStringLiteral("导入文件"),
		QDir::currentPath() + "/Data/",
		QStringLiteral("JSON 文件 (*.json);;所有文件 (*)")
	);

	if (filePath.isEmpty()) return; // 用户取消操作

	LoadCorreDataFromFile(filePath);
}

void HSVCorrectionDialog::closeEvent(QCloseEvent* event)
{
	// 保存数据到文件

	UpdateCurrentSetFromUI();
	m_correDataset.insert(QStringLiteral("历史恢复数据"), m_curData);

	if (!SaveCorreDataToFile("correctionDataset.json")) {
		QMessageBox::warning(this, "warning", QStringLiteral("数据保存失败！"));
		event->ignore();  // 阻止关闭
		return;
	}

	// 接受关闭事件
	event->accept();
}