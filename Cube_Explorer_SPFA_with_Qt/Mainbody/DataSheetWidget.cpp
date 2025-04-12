#include "DataSheetWidget.h"
#include "ui_DataSheetWidget.h"

DataSheetWidget::DataSheetWidget(QSerialPort* serialPort, QWidget* parent) :
    serialPort(serialPort), QDialog(parent), ui(new Ui::DataSheetWidget)
{
    ui->setupUi(this);
    //m_currentSet = ParameterSet();
    //m_keySet = ParaKeySet();

    // 下位机操作
    connect(ui->btn_readPara, &QPushButton::clicked, this, &DataSheetWidget::on_btnReadParaClicked);
    connect(ui->btn_confModi, &QPushButton::clicked, this, &DataSheetWidget::on_btnConfModiClicked);

    // 策略组操作
    connect(ui->btn_saveStrategy, &QPushButton::clicked, this, &DataSheetWidget::on_btnSaveStrategyClicked);
    connect(ui->btn_delStrategy, &QPushButton::clicked, this, &DataSheetWidget::on_btnDelStrategyClicked);
    connect(ui->btn_resetStrategy, &QPushButton::clicked, this, &DataSheetWidget::on_btnResetStrategyClicked);
    connect(ui->list_strategy, &QListWidget::itemDoubleClicked, this, &DataSheetWidget::slot_OpenSelectedStrategy);
    connect(ui->list_strategy, &QListWidget::itemChanged, this, &DataSheetWidget::on_ListItemChanged);
    
    // 串口连接
    connect(serialPort, &QSerialPort::readyRead, this, &DataSheetWidget::slot_ReadParaFromSerial);

    LoadKeyFromUI();
    LoadFromFile("parameters.json");
    UpdateCurrentSetFromUI();
    m_versions.insert(QStringLiteral("默认参数"), m_currentSet);
    m_currentSet = m_versions.value(QStringLiteral("历史恢复数据"));
    ui->list_strategy->setCurrentRow(1);
    UpdateUIFromCurrentSet();
}

DataSheetWidget::~DataSheetWidget()
{
   disconnect(serialPort, &QSerialPort::readyRead, this, &DataSheetWidget::slot_ReadParaFromSerial);
   delete ui;
}

void DataSheetWidget::on_btnReadParaClicked() {
    if (!serialPort->isOpen()) {
        QMessageBox::warning(this, "error", QStringLiteral("串口未打开！"));
        return;
    }
    if (IsCurrentSetDiffFromUI() && IsUserCancelAction(QStringLiteral("你有修改未保存"))) return;
    serialPort->write(QString("#RP0T000\n\r").toLatin1());
    ui->label_message->setText(QStringLiteral("读取成功！"));
}

void DataSheetWidget::on_btnConfModiClicked()
{
    if (!serialPort->isOpen()) {
        QMessageBox::warning(this, "error", QStringLiteral("串口未打开！"));
        return;
    }
    for (int i = 0; i < MAX_PARAM_NUM; i++) {
        //QMessageBox::warning(this, "error", QString("#WP%1T%2\n\r").arg(i, 2).arg(m_currentSet.param[i], 4));
        serialPort->write(QString("#W%1%2\n\r").arg(0, 2, 10, QLatin1Char('0')).arg(m_currentSet.param[0], 4, 10, QLatin1Char('0')).toLatin1());
    }
    serialPort->write("#EP0T000\n\r");
    ui->label_message->setText(QStringLiteral("发送成功！"));
}

void DataSheetWidget::on_btnSaveStrategyClicked()
{
    QString versionName = ui->text_versionName->text().trimmed();
    if (versionName.isEmpty()) {
        QMessageBox::warning(this, "error", QStringLiteral("版本名称不能为空！"));
        return;
    }
    if (m_versions.contains(versionName)) {
        if(IsUserCancelAction(QStringLiteral("策略已存在")))
            return;
    }
    else {
        QListWidgetItem* item = new QListWidgetItem(versionName);
        item->setData(Qt::UserRole, versionName);
        ui->list_strategy->addItem(item);
    }
    UpdateCurrentSetFromUI();
    m_versions.insert(versionName, m_currentSet);
}

void DataSheetWidget::on_btnDelStrategyClicked()
{
    if (ui->list_strategy->currentRow() < 2) {
        QMessageBox::warning(this, "error", QStringLiteral("系统选项无法删除"));
        return;
    }
    if (QListWidgetItem* item = ui->list_strategy->currentItem()) {
        m_versions.remove(item->text());
        delete ui->list_strategy->takeItem(ui->list_strategy->row(item));
    }
    else {
        QMessageBox::warning(this, "warning", QStringLiteral("请先选择要删除的项"));
    }
}

void DataSheetWidget::on_btnResetStrategyClicked()
{
    // reset parameters
    if (IsCurrentSetDiffFromUI() && IsUserCancelAction(QStringLiteral("你有修改未保存"))) return;
    ui->list_strategy->setCurrentRow(0);
    QListWidgetItem* item = ui->list_strategy->currentItem();
    m_currentSet = m_versions[item->text()];
    UpdateUIFromCurrentSet();
}

void DataSheetWidget::on_ListItemChanged(QListWidgetItem* item)
{
    // 获取修改后的文本
    QString newName = item->text().trimmed();

    // 获取该项的原始名称（存储在UserRole中）
    QString oldName = item->data(Qt::UserRole).toString();

    if (m_versions.contains(oldName)) {
        // 获取旧版本数据
        ParameterSet params = m_versions.value(oldName);

        m_versions.remove(oldName);
        m_versions.insert(newName, params);
        item->setData(Qt::UserRole, newName);
    }
    else {
        qWarning() << "找不到对应的版本数据:" << oldName;
    }
}

void DataSheetWidget::slot_OpenSelectedStrategy(QListWidgetItem *item)
{
    if (IsCurrentSetDiffFromUI() && IsUserCancelAction(QStringLiteral("你有修改未保存"))) return;
    m_currentSet = m_versions.value(item->text());
    UpdateUIFromCurrentSet();
}

void DataSheetWidget::slot_ReadParaFromSerial()
{
    QByteArray comByteBuffer = serialPort->readAll();
    if (comByteBuffer.contains("#DS")) {
        while (comByteBuffer.contains("#DS")) {
            comByteBuffer.remove(0, comByteBuffer.indexOf("#DS") + 3);
            if (comByteBuffer.startsWith("END")) {
                UpdateUIFromCurrentSet(); return;
            }
            m_currentSet.SetParameter(comByteBuffer.mid(0, 6).toInt());
        }
    }
}

void DataSheetWidget::LoadKeyFromUI()
{
    m_keySet.key[00] = ui->label_key00->text();
    m_keySet.key[01] = ui->label_key01->text();
    m_keySet.key[02] = ui->label_key02->text();
    m_keySet.key[03] = ui->label_key03->text();
    m_keySet.key[04] = ui->label_key04->text();
    //m_keySet.key[05] = ui->label_key05->text();

    m_keySet.key[10] = ui->label_key10->text();
    m_keySet.key[11] = ui->label_key11->text();
    m_keySet.key[12] = ui->label_key12->text();
    m_keySet.key[13] = ui->label_key13->text();
    m_keySet.key[14] = ui->label_key14->text();

    m_keySet.key[20] = ui->label_key20->text();
    m_keySet.key[21] = ui->label_key21->text();
    m_keySet.key[22] = ui->label_key22->text();
    m_keySet.key[23] = ui->label_key23->text();
    m_keySet.key[24] = ui->label_key24->text();
    m_keySet.key[25] = ui->label_key25->text();
}

void DataSheetWidget::UpdateCurrentSetFromUI()
{
    m_currentSet.param[00] = ui->spin_para00->value();
    m_currentSet.param[01] = ui->spin_para01->value();
    m_currentSet.param[02] = ui->spin_para02->value();
    m_currentSet.param[03] = ui->spin_para03->value();
    m_currentSet.param[04] = ui->spin_para04->value();
    //m_currentSet.param[05] = ui->spin_para05->value();

    m_currentSet.param[10] = ui->spin_para10->value();
    m_currentSet.param[11] = ui->spin_para11->value();
    m_currentSet.param[12] = ui->spin_para12->value();
    m_currentSet.param[13] = ui->spin_para13->value();
    m_currentSet.param[14] = ui->spin_para14->value();

    m_currentSet.param[20] = ui->spin_para20->value();
    m_currentSet.param[21] = ui->spin_para21->value();
    m_currentSet.param[22] = ui->spin_para22->value();
    m_currentSet.param[23] = ui->spin_para23->value();
    m_currentSet.param[24] = ui->spin_para24->value();
    m_currentSet.param[25] = ui->spin_para25->value();
}


void DataSheetWidget::UpdateUIFromCurrentSet()
{
    ui->spin_para00->setValue(m_currentSet.param[00]);
    ui->spin_para01->setValue(m_currentSet.param[01]);
    ui->spin_para02->setValue(m_currentSet.param[02]);
    ui->spin_para03->setValue(m_currentSet.param[03]);
    ui->spin_para04->setValue(m_currentSet.param[04]);

    ui->spin_para10->setValue(m_currentSet.param[10]);
    ui->spin_para11->setValue(m_currentSet.param[11]);
    ui->spin_para12->setValue(m_currentSet.param[12]);
    ui->spin_para13->setValue(m_currentSet.param[13]);
    ui->spin_para14->setValue(m_currentSet.param[14]);

    ui->spin_para20->setValue(m_currentSet.param[20]);
    ui->spin_para21->setValue(m_currentSet.param[21]);
    ui->spin_para22->setValue(m_currentSet.param[22]);
    ui->spin_para23->setValue(m_currentSet.param[23]);
    ui->spin_para24->setValue(m_currentSet.param[24]);
    ui->spin_para25->setValue(m_currentSet.param[25]);
}

bool DataSheetWidget::IsCurrentSetDiffFromUI()
{
    bool isDiff = false;
    isDiff |= (m_currentSet.param[00] != ui->spin_para00->value());
    isDiff |= (m_currentSet.param[01] != ui->spin_para01->value());
    isDiff |= (m_currentSet.param[02] != ui->spin_para02->value());
    isDiff |= (m_currentSet.param[03] != ui->spin_para03->value());
    isDiff |= (m_currentSet.param[04] != ui->spin_para04->value());
    //m_currentSet.param[05] != ui->spin_para05->value();

    isDiff |= (m_currentSet.param[10] != ui->spin_para10->value());
    isDiff |= (m_currentSet.param[11] != ui->spin_para11->value());
    isDiff |= (m_currentSet.param[12] != ui->spin_para12->value());
    isDiff |= (m_currentSet.param[13] != ui->spin_para13->value());
    isDiff |= (m_currentSet.param[14] != ui->spin_para14->value());

    isDiff |= (m_currentSet.param[20] != ui->spin_para20->value());
    isDiff |= (m_currentSet.param[21] != ui->spin_para21->value());
    isDiff |= (m_currentSet.param[22] != ui->spin_para22->value());
    isDiff |= (m_currentSet.param[23] != ui->spin_para23->value());
    isDiff |= (m_currentSet.param[24] != ui->spin_para24->value());
    isDiff |= (m_currentSet.param[25] != ui->spin_para25->value());
    return isDiff;
}

bool DataSheetWidget::IsUserCancelAction(const QString message)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "confirm", message + QStringLiteral("，确定要执行此操作吗？"),
        QMessageBox::Yes | QMessageBox::No);
    return (reply == QMessageBox::No);
}

bool DataSheetWidget::SaveToFile(const QString& filename)
{
    QVariantMap allData;
    for (auto it = m_versions.constBegin(); it != m_versions.constEnd(); ++it) {
        allData.insert(it.key(), m_keySet.toVariantMap(it.value()));
    }
    QString path = "./Data/" + filename;
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonDocument doc(QJsonObject::fromVariantMap(allData));
        file.write(doc.toJson());
        file.close();
        return true;
    }
    return false;
}

void DataSheetWidget::LoadFromFile(const QString& filename)
{
    QString path = "./Data/" + filename;
    QFile file(path);
    m_versions.clear();
    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QVariantMap data = doc.object().toVariantMap();
        for (auto it = data.constBegin(); it != data.constEnd(); ++it) {
            m_versions.insert(it.key(), m_keySet.fromVariantMap(it.value().toMap()));
        }
    }
    if (!m_versions.isEmpty()) {
        for (auto it = m_versions.begin(); it != m_versions.end(); ++it) {
            if (it.key() == QStringLiteral("默认参数") || it.key() == QStringLiteral("历史恢复数据")) continue;
            QListWidgetItem* item = new QListWidgetItem(it.key());
            item->setData(Qt::UserRole, it.key());
            ui->list_strategy->addItem(item);
        }
    }
}

void DataSheetWidget::closeEvent(QCloseEvent* event)
{
    // 保存数据到文件

    UpdateCurrentSetFromUI();
    m_versions.insert(QStringLiteral("历史恢复数据"), m_currentSet);

    if (!SaveToFile("parameters.json")) {
        QMessageBox::warning(this, "warning", QStringLiteral("数据保存失败！"));
        event->ignore();  // 阻止关闭
        return;
    }

    // 接受关闭事件
    event->accept();
}
