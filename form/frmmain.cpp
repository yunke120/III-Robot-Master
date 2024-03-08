#pragma execution_character_set("utf-8")

#include "frmmain.h"
#include "ui_frmmain.h"
#include "core_base/iconhelper.h"
#include "core_base/quihelper.h"

#include <QTime>
#include <QAudioDevice>
#include <QMediaDevices>

frmMain::frmMain(QWidget *parent) : QWidget(parent), ui(new Ui::frmMain)
{
    ui->setupUi(this);
    this->initForm();
    this->initStyle();
    this->initLeftMain();
    this->initLeftConfig();
#if INIT
    this->initSerialPort();
    this->initLogSql();
    this->initMap();
    this->initReport();
    this->initHelp();
    connect(ui->stackedWidget3, &QStackedWidget::currentChanged, this, &frmMain::slotConfigChange);

    pStatusWidget = new StatusWidget(this,
                                     StatusWidget::EnterDirection::LeftIn,
                                     StatusWidget::LeaveDirection::BottomOut);
    pStatusWidget->setStyleSheet("QWidget{border-radius:10px}");
    pStatusWidget->hide();
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    qDebug() << "Qt6";
#else
    qDebug() << "Qt5";
#endif

    const auto devices = QMediaDevices::audioOutputs();
    for (const QAudioDevice &device : devices)
        // qDebug() << "Device: " << device.description();
        ui->cbVoice->addItem(device.description());

}

frmMain::~frmMain()
{
    delete ui;
}

/**
 * @brief 判断文件夹是否存在，不存在就创建
 *
 * @param folder 文件夹
 * @return true 存在或创建成功
 * @return false 不存在或创建失败
 */
bool frmMain::createFolder(const QString &folder)
{
    QDir _dir(folder);
    if(_dir.exists()) return true;
    else return _dir.mkdir(folder);
}

bool frmMain::eventFilter(QObject *watched, QEvent *event)
{
    if (watched == ui->widgetTitle) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            on_btnMenu_Max_clicked();
        }
    }
    return QWidget::eventFilter(watched, event);
}

void frmMain::getQssColor(const QString &qss, const QString &flag, QString &color)
{
    int index = qss.indexOf(flag);
    if (index >= 0) {
        color = qss.mid(index + flag.length(), 7);
    }
    //qDebug() << TIMEMS << flag << color;
}

void frmMain::getQssColor(const QString &qss, QString &textColor, QString &panelColor,
                          QString &borderColor, QString &normalColorStart, QString &normalColorEnd,
                          QString &darkColorStart, QString &darkColorEnd, QString &highColor)
{
    getQssColor(qss, "TextColor:", textColor);
    getQssColor(qss, "PanelColor:", panelColor);
    getQssColor(qss, "BorderColor:", borderColor);
    getQssColor(qss, "NormalColorStart:", normalColorStart);
    getQssColor(qss, "NormalColorEnd:", normalColorEnd);
    getQssColor(qss, "DarkColorStart:", darkColorStart);
    getQssColor(qss, "DarkColorEnd:", darkColorEnd);
    getQssColor(qss, "HighColor:", highColor);
}
#if INIT
void frmMain::appendDatat2LogWidget(const QList<QVariantMap> &data)
{
    ui->logTableWidget->setRowCount(0);
    int numRows = ui->logTableWidget->rowCount();
    ui->logTableWidget->setRowCount(numRows + data.size());

    for (const QVariantMap& log : data) {
        QTableWidgetItem *item_0 = new QTableWidgetItem(QString::number(numRows+1));
        item_0->setTextAlignment(Qt::AlignCenter); // 居中对齐
        ui->logTableWidget->setItem(numRows, 0, item_0);

        QTableWidgetItem *item_1 = new QTableWidgetItem(log["timestamp"].toDateTime().toString());
        item_1->setTextAlignment(Qt::AlignCenter); // 居中对齐
        ui->logTableWidget->setItem(numRows, 1, item_1);

        QTableWidgetItem *item_2 = new QTableWidgetItem(log["level"].toString());
        item_2->setTextAlignment(Qt::AlignCenter); // 居中对齐
        ui->logTableWidget->setItem(numRows, 2, item_2);

        QTableWidgetItem *item_3 = new QTableWidgetItem(log["message"].toString());
//        item_3->setTextAlignment(Qt::AlignCenter); // 居中对齐
        ui->logTableWidget->setItem(numRows, 3, item_3);
        numRows++;
    }
}

void frmMain::appendData2ReportWidget(const QString &data1, const QImage &image)
{
    int rowcount = ui->reportTableWidget->rowCount();
    ui->reportTableWidget->insertRow(rowcount);

    QTableWidgetItem *item1 = new QTableWidgetItem(QString::number(rowcount+1));
    item1->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->reportTableWidget->setItem(rowcount, 0, item1);

    QTableWidgetItem *item2 = new QTableWidgetItem("IPS室巡检");
    item2->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->reportTableWidget->setItem(rowcount, 1, item2);
#if 0
    QDateTime cDateTime = QDateTime::currentDateTime();
    QString cDateTimeStr = cDateTime.toString("yyyy-MM-dd hh:mm:ss");
    QTableWidgetItem *item3 = new QTableWidgetItem(cDateTimeStr);
    item3->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->reportTableWidget->setItem(rowcount, 2, item3);
#endif

    QTableWidgetItem *item4 = new QTableWidgetItem(taskList.at(rowcount));
    item4->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->reportTableWidget->setItem(rowcount, 3, item4);
#if 0
    QLabel *label5 = new QLabel;
    label5->setTextFormat(Qt::MarkdownText);
    label5->setWordWrap(true); // 设置自动换行
    QString text =  "**PA**\n"\
        "  - I1: 004.4\n"\
        "  - I2: 000.0\n"\
        "  - I3: 000.0\n";
    label5->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    label5->setText(text);
    ui->reportTableWidget->setCellWidget(rowcount, 4, label5);
#endif

    QLabel *label6 = new QLabel();
    label6->setPixmap(QPixmap::fromImage(image.scaled(IMAGE_HEIGHT+30, IMAGE_HEIGHT)));
    label6->setAlignment(Qt::AlignHCenter);
    ui->reportTableWidget->setCellWidget(rowcount, 5, label6);

    QTableWidgetItem *item7 = new QTableWidgetItem("正常");
    item7->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->reportTableWidget->setItem(rowcount, 6, item7);



    QWidget *widget = new QWidget();
    QVBoxLayout *layout = new QVBoxLayout();
    QCheckBox *check1 = new QCheckBox("正常");
    QCheckBox *check2 = new QCheckBox("异常");
    layout->addWidget(check1);
    layout->addWidget(check2);
    widget->setLayout(layout);
    ui->reportTableWidget->setCellWidget(rowcount, 7, widget);

#if 1
    static int count;
    count ++;
    if(count == 1)
    {
        // QDateTime cDateTime = QDateTime::currentDateTime();
        // QString cDateTimeStr = cDateTime.toString("yyyy-MM-dd hh:mm:ss");
        QTableWidgetItem *item3 = new QTableWidgetItem("2023-11-21 12:05:22");
        item3->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->reportTableWidget->setItem(rowcount, 2, item3);

        QLabel *label5 = new QLabel;
        label5->setTextFormat(Qt::MarkdownText);
        label5->setWordWrap(true); // 设置自动换行
        QString text =  "**PV**\n"\
            "  - U1: 234.7\n"\
            "  - U2: 236.4\n"\
            "  - U3: 234.5\n";
        label5->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label5->setText(text);
        ui->reportTableWidget->setCellWidget(rowcount, 4, label5);
    }
    else if(count == 2)
    {
        // QDateTime cDateTime = QDateTime::currentDateTime();
        // QString cDateTimeStr = cDateTime.toString("yyyy-MM-dd hh:mm:ss");
        QTableWidgetItem *item3 = new QTableWidgetItem("2023-11-21 12:08:34");
        item3->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->reportTableWidget->setItem(rowcount, 2, item3);

        QLabel *label5 = new QLabel;
        label5->setTextFormat(Qt::MarkdownText);
        label5->setWordWrap(true); // 设置自动换行
        QString text =  "**PV**\n"\
            "  - U1: 224.3\n"\
            "  - U2: 228.7\n"\
            "  - U3: 226.6\n";
        label5->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        label5->setText(text);
        ui->reportTableWidget->setCellWidget(rowcount, 4, label5);


    }

#endif
}
/**
 * @brief 异或校验 (XOR)
 * @param data
 * @param len
 * @return
 */
uint8_t frmMain::checkNumber(const char *data, unsigned char len)
{
    return std::accumulate(data, data + len, static_cast<uint8_t>(0), [](uint8_t acc, uint8_t val) {
        return acc ^ val;
    });
}

void frmMain::_sendCommand(eDEVICE device, unsigned char cmd)
{
    if (!pSerial->isOpen()) {
        pStatusWidget->setShowMessage("提示", "串口未打开");
        pStatusWidget->start();
        LogManager::instance().getLogger()->warn("串口设备未打开！");
        return;
    }
    QByteArray send_msg(PROTOCOL_MSG_LEN, 0);
    send_msg[0] = 0xAA;                                   // 帧头
    send_msg[1] = device;                                 // 设备
    send_msg[2] = cmd;                                    // 命令
    send_msg[10] = checkNumber(send_msg.constData(), 10); //XOR校验位
    send_msg[11] = static_cast<unsigned char>(0xDD);

    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());

    if (pSerial->write(send_msg.data(), 12) == -1) {
        LogManager::instance().getLogger()->error("数据发送失败！(line=%1)",
                                                  QString::number(__LINE__));
    } else {
        if (clickedButton) {
            QString objectName = clickedButton->objectName();
            LogManager::instance().getLogger()->info(objectName);
        }
    }
}

void frmMain::_sendCommand(eDEVICE device,
                           unsigned char cmd,
                           unsigned char *userdata,
                           unsigned char len)
{
    if (!pSerial->isOpen()) {
        pStatusWidget->setShowMessage("提示", "串口未打开");
        pStatusWidget->start();
        LogManager::instance().getLogger()->warn("串口设备未打开！");
        return;
    }

    QByteArray send_msg(PROTOCOL_MSG_LEN, 0);
    send_msg[0] = 0xAA;   // 帧头
    send_msg[1] = device; // 设备
    send_msg[2] = cmd;    // 命令
    send_msg[3] = len;    // 帧长
    std::copy(userdata, userdata + len, send_msg.begin() + 4);
    send_msg[10] = checkNumber(send_msg.data(), 10); // XOR校验位
    send_msg[11] = 0xDD;

    QPushButton *clickedButton = qobject_cast<QPushButton *>(sender());
    if (pSerial->write(send_msg.data(), 12) == -1) {
        LogManager::instance().getLogger()->error("数据发送失败！(line=%1)",
                                                  QString::number(__LINE__));
    } else if (clickedButton) {
        LogManager::instance().getLogger()->info(clickedButton->objectName());
    }
}
#endif
void frmMain::initForm()
{
    //设置无边框
    QUIHelper::setFramelessForm(this);
    //设置图标
    IconHelper::setIcon(ui->labIco, 0xf073, 30);
    IconHelper::setIcon(ui->btnMenu_Min, 0xf068);
    IconHelper::setIcon(ui->btnMenu_Max, 0xf067);
    IconHelper::setIcon(ui->btnMenu_Close, 0xf00d);

    //ui->widgetMenu->setVisible(false);
    ui->widgetTitle->setProperty("form", "title");
    //关联事件过滤器用于双击放大
    ui->widgetTitle->installEventFilter(this);
    ui->widgetTop->setProperty("nav", "top");

    QFont font;
    font.setPixelSize(25);
    ui->labTitle->setFont(font);
    ui->labTitle->setText("变电站智能巡检平台");
    this->setWindowTitle(ui->labTitle->text());

    ui->stackedWidget->setStyleSheet("QLabel{font:16px;} QPushButton{font:16px;}");

    QSize icoSize(32, 32);
    int icoWidth = 85;

    //设置顶部导航按钮
    QList<QAbstractButton *> tbtns = ui->widgetTop->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *btn, tbtns) {
        btn->setIconSize(icoSize);
        btn->setMinimumWidth(icoWidth);
        btn->setCheckable(true);
        connect(btn, SIGNAL(clicked()), this, SLOT(buttonClick()));
    }

    ui->btnMain->click();

    ui->widgetLeftMain->setProperty("flag", "left");
    ui->widgetLeftConfig->setProperty("flag", "left");
    ui->page1->setStyleSheet(QString("QWidget[flag=\"left\"] QAbstractButton{min-height:%1px;max-height:%1px;}").arg(60));
    ui->page2->setStyleSheet(QString("QWidget[flag=\"left\"] QAbstractButton{min-height:%1px;max-height:%1px;}").arg(25));
}

void frmMain::initStyle()
{
    //加载样式表
    QString qss;
    QFile file(":/qss/blacksoft.css");
    if (file.open(QFile::ReadOnly)) {
        qss = QLatin1String(file.readAll());
        QString paletteColor = qss.mid(20, 7);
        qApp->setPalette(QPalette(paletteColor));
        qApp->setStyleSheet(qss);
        file.close();
    }

    //先从样式表中取出对应的颜色
    QString textColor, panelColor, borderColor, normalColorStart, normalColorEnd, darkColorStart, darkColorEnd, highColor;
    getQssColor(qss, textColor, panelColor, borderColor, normalColorStart, normalColorEnd, darkColorStart, darkColorEnd, highColor);

    //将对应颜色设置到控件
    this->borderColor = highColor;
    this->normalBgColor = normalColorStart;
    this->darkBgColor = panelColor;
    this->normalTextColor = textColor;
    this->darkTextColor = normalTextColor;
    QFont font = ui->labelPose->font();
    font.setPointSize(7);
    ui->labelPose->setFont(font);
}

void frmMain::buttonClick()
{
    QAbstractButton *b = (QAbstractButton *)sender();
    QString name = b->text();

    QList<QAbstractButton *> tbtns = ui->widgetTop->findChildren<QAbstractButton *>();
    foreach (QAbstractButton *btn, tbtns) {
        btn->setChecked(btn == b);
    }
    qDebug() << name;
    if (name == "主界面") {
        ui->stackedWidget->setCurrentIndex(0);
    } else if (name == "系统设置") {
        ui->stackedWidget->setCurrentIndex(1);
    } else if (name == "日志查询") {
        ui->stackedWidget->setCurrentIndex(2);
    } else if (name == "调试帮助") {
        ui->stackedWidget->setCurrentIndex(3);
    } else if (name == "用户退出") {
        delete ui->widgetVideo; /* 手动调用析构函数 */
        QThread::sleep(0.3);
        exit(0);
    }
}

void frmMain::initLeftMain()
{
//    iconsMain << 0xf030 << 0xf03e << 0xf247 << 0xe22d << 0xf1b9 ;
//    btnsMain << ui->tbtnMain1 << ui->tbtnMain2 << ui->tbtnMain3 << ui->tbtnMain4 << ui->tbtnMain5;
    iconsMain << 0xf030 << 0xf041 << 0xf247; // << 0xf030 << 0xf072
    btnsMain << ui->tbtnMain1 << ui->tbtnMain2 << ui->tbtnMain3 ;

    int count = btnsMain.count();
    for (int i = 0; i < count; ++i) {
        QToolButton *btn = (QToolButton *)btnsMain.at(i);
        btn->setCheckable(true);
        btn->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(leftMainClick()));
    }

    IconHelper::StyleColor styleColor;
    styleColor.position = "left";
    styleColor.iconSize = 18;
    styleColor.iconWidth = 35;
    styleColor.iconHeight = 25;
    styleColor.borderWidth = 4;
    styleColor.borderColor = borderColor;
    styleColor.setColor(normalBgColor, normalTextColor, darkBgColor, darkTextColor);
    IconHelper::setStyle(ui->widgetLeftMain, btnsMain, iconsMain, styleColor);
    ui->tbtnMain1->click();
}

void frmMain::initLeftConfig()
{
    iconsConfig << 0xf031 << 0xf036 << 0xf249; //
    // iconsConfig<< 0xf055 << 0xf05a << 0xf249;
    btnsConfig << ui->tbtnConfig1 << ui->tbtnConfig2 << ui->tbtnConfig6;

    int count = btnsConfig.count();
    for (int i = 0; i < count; ++i) {
        QToolButton *btn = (QToolButton *)btnsConfig.at(i);
        btn->setCheckable(true);
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        connect(btn, SIGNAL(clicked(bool)), this, SLOT(leftConfigClick()));
    }

    IconHelper::StyleColor styleColor;
    styleColor.position = "left";
    styleColor.iconSize = 16;
    styleColor.iconWidth = 20;
    styleColor.iconHeight = 20;
    styleColor.borderWidth = 3;
    styleColor.borderColor = borderColor;
    styleColor.setColor(normalBgColor, normalTextColor, darkBgColor, darkTextColor);
    IconHelper::setStyle(ui->widgetLeftConfig, btnsConfig, iconsConfig, styleColor);
    ui->tbtnConfig1->click();
}

void frmMain::leftMainClick()
{
    QAbstractButton *b = (QAbstractButton *)sender();
    QString name = b->text();

    int count = btnsMain.count();
    for (int i = 0; i < count; ++i) {
        QAbstractButton *btn = btnsMain.at(i);
        btn->setChecked(btn == b);
    }

    if(name == "视频监控"){
        ui->stackedWidget2->setCurrentIndex(0);
    } else if (name == "地图监控") {
        ui->stackedWidget2->setCurrentIndex(1);
    } else if (name == "设备监控") {
        ui->stackedWidget2->setCurrentIndex(2);
    }
}
void frmMain::leftConfigClick()
{
    QToolButton *b = (QToolButton *)sender();
    QString name = b->text();

    int count = btnsConfig.count();
    for (int i = 0; i < count; ++i) {
        QAbstractButton *btn = btnsConfig.at(i);
        btn->setChecked(btn == b);
        if(btn == b)
            ui->stackedWidget3->setCurrentIndex(i);
    }

}
#if INIT
void frmMain::initSerialPort()
{
    pSerial = new QSerialPort(this);
    pSerial->setBaudRate(QSerialPort::Baud115200);
    pSerial->setDataBits(QSerialPort::Data8);
    pSerial->setStopBits(QSerialPort::OneStop);
    pSerial->setParity(QSerialPort::NoParity);
    pSerial->setFlowControl(QSerialPort::NoFlowControl);
    connect(pSerial, &QSerialPort::readyRead, this, &frmMain::slotSerialReadyRead);

    ui->cbPorts->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) /* 初始化串口列表 */
    {
        ui->cbPorts->addItem(info.portName());
    }

    pDataTimer = new QTimer(this);
    pDataTimer->setInterval(10);
    connect(pDataTimer, &QTimer::timeout, this, &frmMain::slotDataTimerOut);
}

void frmMain::initConfig()
{
    QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
}

void frmMain::initLogSql()
{
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString currentDate = currentDateTime.toString("yyyy-MM-dd");

    // 检查是否需要生成新的数据库文件
    static QString currentDbName;
    static QString logsDirectory = "logs";
    if (currentDate != currentDbName) {
        currentDbName = logsDirectory + "/" + currentDate + ".db";
    }


    plogSql = new LoggerSql(currentDbName);

    // 记录一些日志
   // plogSql->log(LoggerSql::eINFO, "This is an informational message.");
   // plogSql->log(LoggerSql::eERROR, "An error occurred.");
   // plogSql->log(LoggerSql::eINFO, "Another informational message.");
   // plogSql->log(LoggerSql::eWARN, "中文测试1234567890");
   // plogSql->log(LoggerSql::eINFO, "This is an informational message.");
   // plogSql->log(LoggerSql::eERROR, "An error occurred.");
   // plogSql->log(LoggerSql::eINFO, "Another informational message.");
   // plogSql->log(LoggerSql::eWARN, "中文测试1234567890");
   // plogSql->log(LoggerSql::eWARN, "中文测试1234567890");
   // plogSql->log(LoggerSql::eINFO, "This is an informational message.");
   // plogSql->log(LoggerSql::eERROR, "An error occurred.");
   // plogSql->log(LoggerSql::eINFO, "Another informational message.");
   // plogSql->log(LoggerSql::eWARN, "中文测试1234567890");
   // plogSql->log(LoggerSql::eINFO, "This is an informational message.");
   // plogSql->log(LoggerSql::eERROR, "An error occurred.");
   // plogSql->log(LoggerSql::eINFO, "Another informational message.");
   // plogSql->log(LoggerSql::eWARN, "中文测试1234567890");
   // plogSql->log(LoggerSql::eINFO, "This is an informational message.");
   // plogSql->log(LoggerSql::eERROR, "An error occurred.");
   // plogSql->log(LoggerSql::eINFO, "Another informational message.");
   // plogSql->log(LoggerSql::eWARN, "中文测试1234567890");
   // plogSql->log(LoggerSql::eWARN, "中文测试1234567890");
   // plogSql->log(LoggerSql::eINFO, "This is an informational message.");
   // plogSql->log(LoggerSql::eERROR, "An error occurred.");
   // plogSql->log(LoggerSql::eINFO, "Another informational message.");
   // plogSql->log(LoggerSql::eWARN, "中文测试1234567890");

/**
 * 1. 系统初始化
 * 2. 通信连接成功
 * 3. 视频流接收成功
 * 4. 导入地图文件IPS室地图
 * 5. 添加巡检点，任务1：1号直流馈线盘
 * 6. 添加巡检点，任务2：1号直流馈线盘
 * 7. 修改巡检点，任务2：2号直流馈线盘
 * 8. 添加巡检点，任务3：1号直流馈线盘
 * 9. 修改巡检点，任务3：3号直流馈线盘
 * 10. 添加巡检点，任务4：1号直流馈线盘
 * 11. 修改巡检点，任务4：通信及事故照明盘
 * 12. 添加巡检点，任务5：1号直流馈线盘
 * 13. 修改巡检点，任务5：UPS不间断电源盘
 * 12. 添加巡检点，任务6：1号直流馈线盘
 * 13. 修改巡检点，任务6：调度数据专用盘1
 * 12. 添加巡检点，任务7：1号直流馈线盘
 * 13. 修改巡检点，任务7：1号交流馈线盘
 * 12. 添加巡检点，任务8：1号直流馈线盘
 * 13. 修改巡检点，任务8：2号交流馈线盘
 *
*/

    // 获取并打印所有日志
   // QList<QVariantMap> allLogs = plogSql->getLogs();
   // qDebug() << "All Logs:";
   // for (const QVariantMap& log : allLogs) {
   //     qDebug() << "ID: " << log["id"].toInt()
   //              << "Timestamp: " << log["timestamp"].toDateTime().toString()
   //              << "Level: " << log["level"].toString()
   //              << "Message: " << log["message"].toString();
   // }



    ui->logTableWidget->verticalHeader()->setVisible(false);
    // ui->logTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 调整表格列宽

    // 设置表头
    QStringList headers;
    headers << "条目" << "时间" << "日志等级" << "消息";
    ui->logTableWidget->setColumnCount(4);
    ui->logTableWidget->setHorizontalHeaderLabels(headers);
    // 设置列的stretch属性，让它们自动拉伸
    ui->logTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Interactive);
    ui->logTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Interactive);
    ui->logTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->logTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
}

void frmMain::initMap()
{
    connect(this, &frmMain::sig_ImportMap, ui->graphicsViewMap, &MapWidget::showMap);
    connect(this, &frmMain::sigSetRobotPose, ui->graphicsViewMap, &MapWidget::slotSetRobotPose,Qt::QueuedConnection);
    connect(this, &frmMain::sigSetGoalStatus, ui->graphicsViewMap, &MapWidget::slotSetGoalStatus);

    ui->taskTableWidget->verticalHeader()->setVisible(false);
    ui->taskTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 调整表格列宽

    // 设置表头
    QStringList headers;
    headers << "序号" <<  "巡航点位" << "任务状态" << "删除任务" << "运行任务";
    ui->taskTableWidget->setColumnCount(headers.size());
    ui->taskTableWidget->setHorizontalHeaderLabels(headers);
    // 设置列的stretch属性，让它们自动拉伸
    for(int i = 0; i < headers.size(); i++)
    {
        if(0==i)
        {
            ui->taskTableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::ResizeToContents);
            continue;
        }
        ui->taskTableWidget->horizontalHeader()->setSectionResizeMode(i, QHeaderView::Stretch);
    }

    QFile file("./task.txt");
    if(file.open(QFileDevice::ReadOnly))
    {
        QTextStream in(&file);
        while (!in.atEnd())
        {
            QString line = in.readLine();
            taskList << line;
        }
        file.close();
    }
    else
    {
        LogManager::instance().getLogger()->error("初始化任务列表失败，请检查有无task.txt文件");
    }

    connect(ui->hVelSlider2, &QSlider::valueChanged, [this](int value){
        ui->labelVel2->setText(QString("%1m/s").arg(value/100.0, 2));
    });
    connect(ui->hRadSlider2, &QSlider::valueChanged, [this](int value){
        ui->labelRad2->setText(QString("%1rad/s").arg(value/100.0, 2));
    });

    connect(ui->hSliderVelocity, &QSlider::valueChanged, [this](int value){
        ui->labelVel->setText(QString("%1m/s").arg(value/100.0, 2));

    });
    connect(ui->hSliderVelocity, &QSlider::sliderReleased, [this](){

        union UInt16Union data;
        data.value = ui->hSliderVelocity->value() * 10;

        unsigned char userdata[2] = {data.parts.lowByte, data.parts.highByte};
        _sendCommand(Robot, RobotVelocity, userdata, 2);

    });
    connect(ui->hSliderRad, &QSlider::valueChanged, [this](int value){
        ui->labelRad->setText(QString("%1rad/s").arg(value/100.0, 2));
    });
}

void frmMain::initReport()
{
    ui->reportTableWidget->verticalHeader()->setVisible(false);
    // ui->reportTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    // 调整表格列宽

    // 设置表头
    QStringList headers;
    headers << "序号" << "监控对象" << "巡检时间" << "巡检设备" << "分析结果" << "巡检图片" << "报警状态" << "人工审核" << "描述";
    ui->reportTableWidget->setColumnCount(headers.size());
    ui->reportTableWidget->setHorizontalHeaderLabels(headers);
    // 设置列的stretch属性，让它们自动拉伸
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(6, QHeaderView::Stretch);
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(7, QHeaderView::Stretch);
    ui->reportTableWidget->horizontalHeader()->setSectionResizeMode(8, QHeaderView::Stretch);

    ui->reportTableWidget->verticalHeader()->setDefaultSectionSize(IMAGE_HEIGHT);

    connect(ui->reportTableWidget, &QTableWidget::cellDoubleClicked, [this](int row, int col){
        // QLabel *label = qobject_cast<QLabel *>(ui->reportTableWidget->cellWidget(row, col));
        // QPixmap pix = label->pixmap();
        QPixmap pix("D:/III-Lab/_Paper/diagrams/qt/meters/PV1.png");
        QDialog *dialog = new QDialog(this);
        dialog->setFixedSize(QSize(800,600));
        QGridLayout *layout = new QGridLayout(dialog);
        QLabel *imgLabel = new QLabel(dialog);
        imgLabel->setPixmap(pix.scaled(800, 600, Qt::KeepAspectRatioByExpanding));
        QPushButton *btn1 = new QPushButton("异常", dialog);
        QPushButton *btn2 = new QPushButton("正常", dialog);
        layout->addWidget(imgLabel, 0,0,1,2);
        layout->addWidget(btn1, 1, 0);  //添加"异常"按钮到网格布局
        layout->addWidget(btn2, 1, 1);  //添加"正常"按钮到网格布局
        dialog->setLayout(layout);  //对话框设置布局
        dialog->show();
    });
}

void frmMain::initHelp()
{
    ui->labelHelpDoc->setTextFormat(Qt::MarkdownText);
    ui->labelHelpDoc->setWordWrap(true);
    QFile helpFile("./HELP.md");

    if(helpFile.open(QIODevice::ReadOnly))
        ui->labelHelpDoc->setText(helpFile.readAll());
    helpFile.close();

    // ui->labelFAQ->setTextFormat(Qt::MarkdownText);
    // ui->labelFAQ->setWordWrap(true);
    // QFile faqFile("./FAQ.md");

    // if(faqFile.open(QIODevice::ReadOnly))
    //     ui->labelFAQ->setText(faqFile.readAll());
    // faqFile.close();
}
#endif
void frmMain::on_btnMenu_Min_clicked()
{
    showMinimized();
}

void frmMain::on_btnMenu_Max_clicked()
{
    static bool max = false;
    static QRect location = this->geometry();

    if (max) {
        this->setGeometry(location);
    } else {
        location = this->geometry();
        this->setGeometry(QUIHelper::getScreenRect());
    }

    this->setProperty("canMove", max);
    max = !max;
}

void frmMain::on_btnMenu_Close_clicked()
{
    close();
}
#if INIT
void frmMain::on_btnOpenVideo_clicked()
{
    if (ui->btnOpenVideo->text() == "打开视频") {
        ui->btnOpenVideo->setText("关闭视频");
        QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
        QString videoaddr = settings.value("Config2/videoaddr").toString();
        qDebug()<< videoaddr;
        ui->widgetVideo->setAddr(videoaddr); // 网络流
        //        ui->widgetVideo->setAddr("rtsp://192.168.1.210/H264LiveStream"); // 本地视频文件
        //        ui->widgetVideo->setAddr("rtsp://192.168.2.119/554"); // 本地视频文件
        // ui->widgetVideo->setAddr(0); // 本地摄像头
        ui->widgetVideo->setEnablePython(false);
        ui->widgetVideo->open();
    } else {
        ui->btnOpenVideo->setText("打开视频");
        ui->widgetVideo->close();
    }
}

/**
 * @brief 对接收到的12个字节进行重新排序，主要解决接收乱序的问题
 *
 * @param array 串口接收的一帧数据
 */
void frmMain::reorderFrameData(QByteArray &array)
{
    int frameHead = array.indexOf(0xAA);
    if(frameHead == 0 && array.at(11) == 0xDD) return;

        QByteArray temp = array.mid(0, frameHead);
        array.remove(0, frameHead);
        array.append(temp);
}

/**
 * @brief 串口中断函数
 */
void frmMain::slotSerialReadyRead()
{
    QByteArray buf = pSerial->readAll();
    //    qDebug() << buf;
    AllArray.append(buf);

    QByteArray array;
    int i;
    for (i = 0; i < AllArray.size() / 12; i++) {
        array = AllArray.mid(12 * i, 12);
        reorderFrameData(array);
        mRecvQueue.enqueue(array);
#if (1)
        QString str;
        for (int i = 0; i < 12; i++) {
            str += QString("%1 ").arg((unsigned char) array.at(i), 2, 16, QChar('0'));
        }
        // ui->tELog->append(str);
        qDebug() << str;
#endif
    }
    AllArray.remove(0, 12 * i);
    //    qDebug() << mRecvQueue.size();
}

void frmMain::slotDataTimerOut()
{
   if(mRecvQueue.isEmpty()) return;

   QByteArray array = mRecvQueue.dequeue();

   if(array.size() == 12)
   {
       if(static_cast<unsigned char>(array.at(0)) == FrameHeader && static_cast<unsigned char>(array.at(11)) == FrameTail)
       {
           switch (eDEVICE(array.at(1)))
           {
           case Robot:
               if(static_cast<unsigned char>(array.at(2)) == eRobot::RobotVelocity)
               {
                   union UInt16Union data;
                   data.parts.lowByte = static_cast<unsigned char>(array.at(4));
                   data.parts.highByte = static_cast<unsigned char>(array.at(5));
                   // qDebug() << "v: " + QString::number(data.value/1000.0);
                   qreal value = data.value / 10.0;
                   // qDebug() << "veocity = " << value;
                   ui->VelWidget->setValue(value);
               }
               else if(static_cast<unsigned char>(array.at(2)) == eRobot::HandShake)
               {
                   ui->tBConnect->setText("在线");
                   ui->tBConnect->setIcon(QIcon(":/image/connect.png"));
               }
               else {
                   qDebug("error command 1.");
               }
               break;
           case Battery:
               if(static_cast<unsigned char>(array.at(2)) == eBattery::GetVoltage)
               {
                   union UInt16Union data;
                   data.parts.lowByte = static_cast<unsigned char>(array.at(4));
                   data.parts.highByte = static_cast<unsigned char>(array.at(5));

                   qDebug() << "bat: " + QString::number(data.value/1000.0);
                   ui->tBBattery->setText(QString("%1V").arg(data.value/1000.0, 3));
               }
               else {
                   qDebug("error command 2.");
               }
               break;
           case Sensor:

               break;

           default:
               qDebug("no message.");
               break;
           }
       }
       else {
           qDebug( "error header or tail.");
       }
   }
   else
   {
       qDebug("error len.");
   }
}

void frmMain::slotConfigChange(int index)
{
    QString group = QString("Config%1").arg(index+1);
    qDebug() << group;
    QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
    if(index == 0)
    {
        // int _1 = settings.value("Config1/Language").toInt();
        // int _2 = settings.value("Config1/Theme").toInt();
        QString configSavePath = settings.value("Config1/configSavePath").toString();
        QString imageSavePath = settings.value("Config1/imageSavePath").toString();
        QString videoSavePath = settings.value("Config1/videoSavePath").toString();

        // ui->cbLanguage->setCurrentIndex(_1);
        // ui->cbTheme->setCurrentIndex(_2);
        ui->leSaveConfig->setText(configSavePath);
        ui->leSaveImage->setText(imageSavePath);
        ui->leSaveVideo->setText(videoSavePath);
    }
    else if(index == 1)
    {
        QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
        QString port = settings.value("Config2/port").toString();
        int databit = settings.value("Config2/databit").toInt();
        int bandrate = settings.value("Config2/bandrate").toInt();
        int stopbit = settings.value("Config2/stopbit").toInt();
        int parity = settings.value("Config2/parity").toInt();
        QString videoaddr = settings.value("Config2/videoaddr").toString();

        ui->cbPorts->setCurrentText(port);
        ui->cbDatas->setCurrentIndex(databit);
        ui->cbBandrate->setCurrentIndex(bandrate);
        ui->cbStops->setCurrentIndex(stopbit);
        ui->cbParity->setCurrentIndex(parity);
        ui->leVideoAddr->setText(videoaddr);
    }
}

/**
 * @brief 控制机器人前进
 */
void frmMain::on_btnRobotUp_clicked()
{
    _sendCommand(Robot, Robot_Foreward);
}

/**
 * @brief 控制机器人后退
 */
void frmMain::on_btnRobotDown_clicked()
{
    _sendCommand(Robot, Robot_Backward);
}

/**
 * @brief 控制机器人左移
 */
void frmMain::on_btnRobotLeft_clicked()
{
    _sendCommand(Robot, Robot_Shift_Left);
}

/**
 * @brief 控制机器人右移
 */
void frmMain::on_btnRobotRight_clicked()
{
    _sendCommand(Robot, Robot_Shift_Right);
}

void frmMain::on_btnRobotLeftUp_clicked()
{
    _sendCommand(Robot, Robot_Left_Up);
}

void frmMain::on_btnRobotRightUp_clicked()
{
    _sendCommand(Robot, Robot_Right_Up);
}

void frmMain::on_btnRobotLeftDown_clicked()
{
    _sendCommand(Robot, Robot_Left_Down);
}

void frmMain::on_btnRobotRightDown_clicked()
{
    _sendCommand(Robot, Robot_Right_Down);
}

/**
 * @brief 控制机器人停止
 */
void frmMain::on_btnRobotStop_clicked()
{
    _sendCommand(Robot, Robot_Stop);
}

// void frmMain::on_btnRobotAccelerate_clicked()
// {
    // mDefaultVelocity += 1;
    // if (mDefaultVelocity >= 10)
    //     mDefaultVelocity = 10;
    // ui->labelRobotVelocity->setText(
    //     QString("%1m/s").arg(static_cast<double>(mDefaultVelocity / 10.0), 3));

    // union UInt16Union data;
    // data.value = short(mDefaultVelocity / 10.0 * 1000);

    // unsigned char userdata[2] = {data.parts.lowByte, data.parts.highByte};
    // _sendCommand(Robot, RobotVelocity, userdata, 2);
// }
//
// void frmMain::on_btnDecelerate_clicked()
// {
    // mDefaultVelocity -= 1;
    // if (mDefaultVelocity <= 0)
    //     mDefaultVelocity = 0;
    // ui->labelRobotVelocity->setText(
    //     QString("%1m/s").arg(static_cast<double>(mDefaultVelocity / 10.0), 3));
    // union UInt16Union data;
    // data.value = short(mDefaultVelocity / 10.0 * 1000);

    // unsigned char userdata[2] = {data.parts.lowByte, data.parts.highByte};
    // _sendCommand(Robot, RobotVelocity, userdata, 2);
// }

/**
 * @brief frmMain::on_btnSaveConfig_clicked
 */
void frmMain::on_btnSaveConfig_clicked()
{
   QString filepath = QFileDialog::getOpenFileName(this, "选择配置文件", "./", "INI Files(*.ini)",nullptr,QFileDialog::DontUseNativeDialog);
   if(filepath.isEmpty()) return;
   ui->leSaveConfig->setText(filepath);
}

void frmMain::on_btnSaveImage_clicked()
{
   QString path = QFileDialog::getExistingDirectory(this, "设置截图保存路径", "./", QFileDialog::DontUseNativeDialog);
   if(path.isEmpty()) return;

   ui->leSaveImage->setText(path);
}

void frmMain::on_btnSaveVideo_clicked()
{
   QString path = QFileDialog::getExistingDirectory(this, "设置录像保存路径", "./", QFileDialog::DontUseNativeDialog);
   if(path.isEmpty()) return;

   ui->leSaveVideo->setText(path);
}

void frmMain::on_btnC1Cancel_clicked()
{
   slotConfigChange(0);
}


void frmMain::on_btnC1Apply_clicked()
{
   QString _configSavePath = ui->leSaveConfig->text();
   QString _imageSavePath = ui->leSaveImage->text();
   QString _videoSavePath = ui->leSaveVideo->text();

   QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
   settings.beginGroup("Config1");
   // settings.setValue("Language", ui->cbLanguage->currentIndex());
   // settings.setValue("Theme", ui->cbTheme->currentIndex());
   settings.setValue("configSavePath", _configSavePath);
   settings.setValue("imageSavePath", _imageSavePath);
   settings.setValue("videoSavePath", _videoSavePath);
   settings.endGroup();

   if(settings.status() != QSettings::NoError)
   {
       QMessageBox::warning(this, "警告", "设置保存失败");
   }
   else
   {
       QMessageBox::information(this, "提示", "应用成功");
   }
}

void frmMain::on_btnC1Confirm_clicked()
{
   QString _configSavePath = ui->leSaveConfig->text();
   QString _imageSavePath = ui->leSaveImage->text();
   QString _videoSavePath = ui->leSaveVideo->text();

   QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
   settings.beginGroup("Config1");
   settings.setValue("configSavePath", _configSavePath);
   settings.setValue("imageSavePath", _imageSavePath);
   settings.setValue("videoSavePath", _videoSavePath);
   settings.endGroup();

   if(settings.status() != QSettings::NoError)
   {
       QMessageBox::warning(this, "警告", "设置保存失败");
   }
   else
   {
       QMessageBox::information(this, "提示", "保存成功");
   }
}

void frmMain::on_btnC2Cancel_clicked()
{
//    slotConfigChange(1);
}

void frmMain::on_btnC2Confirm_clicked()
{
    QString port = ui->cbPorts->currentText();
    int databit = ui->cbDatas->currentIndex();
    int bandrate = ui->cbBandrate->currentIndex();
    int stopbit = ui->cbStops->currentIndex();
    int parity = ui->cbParity->currentIndex();
    QString videoaddr = ui->leVideoAddr->text();

    QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
    settings.beginGroup("Config2");
    settings.setValue("port", port);
    settings.setValue("databit", QString::number(databit));
    settings.setValue("bandrate", QString::number(bandrate));
    settings.setValue("stopbit", QString::number(stopbit));
    settings.setValue("parity", QString::number(parity));
    settings.setValue("videoaddr", videoaddr);
    settings.endGroup();
    if (settings.status() != QSettings::NoError) {
        QMessageBox::warning(this, "警告", "设置保存失败");
    } else {
        QMessageBox::information(this, "提示", "保存成功");
    }
}

void frmMain::on_btnRefreshPort_clicked()
{
    ui->cbPorts->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) /* 初始化串口列表 */
    {
        ui->cbPorts->addItem(info.portName());
    }
}



void frmMain::on_btnOpenSerial_clicked()
{
    if (ui->btnOpenSerial->text() == "打开通信") {
        QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
        QString port = settings.value("Config2/port").toString();
        pSerial->setPortName(port);
        if (!pSerial->open(QIODevice::ReadWrite)) {
            pStatusWidget->setShowMessage("警告", pSerial->errorString());
            pStatusWidget->start();
            return;
        }
        ui->btnOpenSerial->setText("断开通信");
        pDataTimer->start();
        pStatusWidget->setShowMessage("提示", "打开串口成功");
        pStatusWidget->start();

        LogManager::instance().getLogger()->info("打开串口成功");
        ui->tBConnect->setText("在线");
        ui->tBConnect->setIcon(QIcon(":/image/connect.png"));
    } else {
        if (!pSerial->isOpen())
            return;
        pSerial->close();
        ui->btnOpenSerial->setText("打开通信");
        if (pDataTimer->isActive())
            pDataTimer->stop();
        LogManager::instance().getLogger()->info("关闭串口");
        ui->tBConnect->setText("离线");
        ui->tBConnect->setIcon(QIcon(":/image/disconnect.png"));
    }
}



void frmMain::on_tbFilter_clicked()
{
   int level = ui->cbLogLevel->currentIndex();
   LoggerSql::LogLevel _level = static_cast<LoggerSql::LogLevel>(level);
   QList<QVariantMap> data = plogSql->getLogs(_level);
   appendDatat2LogWidget(data);
}

void frmMain::on_tbSelectLogFile_clicked()
{
    // 获取并打印特定等级的日志
    // QList<QVariantMap> errorLogs = plogSql->getLogs(LoggerSql::LogLevel::eWARN);
    // qDebug() << errorLogs.length();
    // qDebug() << "Error Logs:";
    // for (const QVariantMap& log : errorLogs) {
    //     qDebug() << "ID: " << log["id"].toInt()
    //              << "Timestamp: " << log["timestamp"].toDateTime().toString()
    //              << "Level: " << log["level"].toString()
    //              << "Message: " << log["message"].toString();
    // }
    QString sqlName = QFileDialog::getOpenFileName(this, "选择数据库文件", "./logs");
    QDir dir(QCoreApplication::applicationDirPath());
    QString relativePath = dir.relativeFilePath(sqlName);
    ui->leLogFile->setText(relativePath);

    int level = ui->cbLogLevel->currentIndex();

    // LoggerSql *_logSql = new LoggerSql(sqlName);

    QList<QVariantMap> errorLogs = plogSql->getLogs(LoggerSql::LogLevel(level));

    for (const QVariantMap& log : errorLogs) {
        qDebug() << "ID: " << log["id"].toInt()
                 << "Timestamp: " << log["timestamp"].toDateTime().toString()
                 << "Level: " << log["level"].toString()
                 << "Message: " << log["message"].toString();

        int rowcount = ui->logTableWidget->rowCount();
        ui->logTableWidget->insertRow(rowcount);

        QTableWidgetItem *item0 = new QTableWidgetItem(log["id"].toString());
        item0->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->logTableWidget->setItem(rowcount, 0, item0);

        QTableWidgetItem *item1 = new QTableWidgetItem(log["timestamp"].toDateTime().toString());
        item1->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->logTableWidget->setItem(rowcount, 1, item1);

        QTableWidgetItem *item2 = new QTableWidgetItem(log["level"].toString());
        item2->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
        ui->logTableWidget->setItem(rowcount, 2, item2);

        QTableWidgetItem *item3 = new QTableWidgetItem(log["message"].toString());
        ui->logTableWidget->setItem(rowcount, 3, item3);
    }
}





void frmMain::on_btnGPU_clicked()
{
    QString  command( "cmd.exe" );
//    QStringList params = QStringList() << "/k" << "ping" << "1.1.1.1";
//    QStringList params = QStringList() << "/k" << "echo" << "%cd%";
    QStringList params = QStringList() << "/k" << "python" << "./yolov5/detect.py";

    QProcess::startDetached( command, params );
}
#endif

void frmMain::on_btnImportMap_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Select Map",
                                                    "./maps",
                                                    "Map(*.pgm *png)");
    emit sig_ImportMap(filename);
}


void frmMain::on_btnAddGoalPoint_clicked()
{
    int rowcount = ui->taskTableWidget->rowCount();
    ui->taskTableWidget->insertRow(rowcount);

    QTableWidgetItem *Item0 = new QTableWidgetItem(QString::number(rowcount+1));
    Item0->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->taskTableWidget->setItem(rowcount, 0, Item0);

    QComboBox *combox = new QComboBox();
    combox->addItems(taskList);
    ui->taskTableWidget->setCellWidget(rowcount, 1, combox);

    connect(combox, static_cast<void(QComboBox::*)(const QString &)>(&QComboBox::currentTextChanged), [this, combox, rowcount](const QString &text) {
        // qDebug() << "row: " << rowcount << "index: " << index;
        qDebug() << text;
        int index = taskList.indexOf(text);
        emit sigSetGoalStatus(index+1, GoalItem::GOALTYPE::Undone);
    });

    QTableWidgetItem *taskStatusItem = new QTableWidgetItem("未巡检");
    taskStatusItem->setTextAlignment(Qt::AlignVCenter | Qt::AlignHCenter);
    ui->taskTableWidget->setItem(rowcount, 2, taskStatusItem);

    QPushButton *btnDelete = new QPushButton("删除");
    ui->taskTableWidget->setCellWidget(rowcount, 3, btnDelete);

    QPushButton *btnRun = new QPushButton("运行");
    ui->taskTableWidget->setCellWidget(rowcount, 4, btnRun);

    QString style = "QPushButton{\
                    border-radius:1px;\
                    color:#DCDCDC;\
                    padding:1px;\
                    margin:0px;\
                    background:none;\
                    border-style:none;}";
    btnDelete->setStyleSheet(style);
    btnRun->setStyleSheet(style);

    emit sigSetGoalStatus(1, GoalItem::GOALTYPE::Undone);
}

void frmMain::on_btnSaveTaskList_clicked()
{
    int rowcount = ui->taskTableWidget->rowCount();
    for(int i = 0; i < rowcount; i++)
    {
        QString str = qobject_cast<QComboBox*>(ui->taskTableWidget->cellWidget(i, 0))->currentText();
        qDebug()<< str;
    }
}


void frmMain::on_btnImportTaskList_clicked()
{

}


void frmMain::on_btnExportReport_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
                                                    "Select IMAGE",
                                                    "./",
                                                    "Map(*.jpg *png)");
    qDebug() << filename;
    if(filename == "") return;
    qDebug() << filename;
    QImage image(filename);

    appendData2ReportWidget("通信及事故照明盘", image);
}


void frmMain::on_btnRealTimeDetect_clicked(bool checked)
{

}


void frmMain::on_btnRobotLeftUp2_clicked()
{
    QTableWidgetItem *item1 = ui->taskTableWidget->item(0,2);
    item1->setText("已巡检");



    for(int i = 0; i<140;i++)
    {
        std::random_device rd;  //用于获取种子数据
        std::mt19937 gen(rd()); //使用Mersenne Twister算法的随机数生成器
        std::uniform_int_distribution<> dis(180, 181); //定义一个区间，生成这个区间内的随机数

        int randomValue = dis(gen); //生成随机数
        // QThread::msleep(100);
        emit sigSetRobotPose(160-i,randomValue,180); //1
    }
    emit sigSetGoalStatus(1, GoalItem::GOALTYPE::Done);
    emit sigSetGoalStatus(2, GoalItem::GOALTYPE::Done);
    emit sigSetGoalStatus(3, GoalItem::GOALTYPE::Done);
    emit sigSetGoalStatus(9, GoalItem::GOALTYPE::Done);
    // emit sigSetGoalStatus(1, GoalItem::GOALTYPE::Done);
    ui->VelWidget->setValue(50);
}


void frmMain::on_btnRobotUp2_clicked()
{
    for(int i = 0; i<40;i++)
    {
        std::random_device rd;  //用于获取种子数据
        std::mt19937 gen(rd()); //使用Mersenne Twister算法的随机数生成器
        std::uniform_int_distribution<> dis(20, 21); //定义一个区间，生成这个区间内的随机数

        int randomValue = dis(gen); //生成随机数
        // QThread::msleep(100);
        emit sigSetRobotPose(randomValue,180-i,180); //1
    }

    // emit sigSetRobotPose(100,180,180); //2
    // emit sigSetGoalStatus(2, GoalItem::GOALTYPE::Done);
    QTableWidgetItem *item2 = ui->taskTableWidget->item(2,2);
    item2->setText("已巡检");
    QTableWidgetItem *item3 = ui->taskTableWidget->item(3,2);
    item3->setText("已巡检");
    QTableWidgetItem *item4 = ui->taskTableWidget->item(1,2);
    item4->setText("已巡检");
}


void frmMain::on_btnRobotRightUp2_clicked()
{

    for(int i = 0; i<90;i++)
    {
        std::random_device rd;  //用于获取种子数据
        std::mt19937 gen(rd()); //使用Mersenne Twister算法的随机数生成器
        std::uniform_int_distribution<> dis(140, 141); //定义一个区间，生成这个区间内的随机数

        int randomValue = dis(gen); //生成随机数
        // QThread::msleep(100);
        emit sigSetRobotPose(20+i,randomValue,0); //1
    }
    // emit sigSetRobotPose(80,180,180); //3
    // emit sigSetGoalStatus(3, GoalItem::GOALTYPE::Done);
}


void frmMain::on_btnRobotLeft2_clicked()
{
    emit sigSetRobotPose(60,180,180); //9
    emit sigSetGoalStatus(9, GoalItem::GOALTYPE::Done);
}


void frmMain::on_btnRobotRight2_clicked()
{
    emit sigSetRobotPose(120,140,0); //9
    emit sigSetGoalStatus(10, GoalItem::GOALTYPE::Done);
}


void frmMain::on_btnRobotLeftDown2_clicked()
{
    for(int i = 0; i<58;i++)
    {
        std::random_device rd;  //用于获取种子数据
        std::mt19937 gen(rd()); //使用Mersenne Twister算法的随机数生成器
        std::uniform_int_distribution<> dis(114, 115); //定义一个区间，生成这个区间内的随机数

        int randomValue = dis(gen); //生成随机数
        emit sigSetRobotPose(58+i,randomValue,0); //1
    }
}

void frmMain::on_btnRobotDown2_clicked()
{
    for(int i = 0; i<69;i++)
    {
        std::random_device rd;  //用于获取种子数据
        std::mt19937 gen(rd()); //使用Mersenne Twister算法的随机数生成器
        std::uniform_int_distribution<> dis(116, 117); //定义一个区间，生成这个区间内的随机数

        int randomValue = dis(gen); //生成随机数
        emit sigSetRobotPose(randomValue,114-i,270); //1
    }
}


void frmMain::on_btnRobotRightDown2_clicked()
{
    for(int i = 0; i<49;i++)
    {
        std::random_device rd;  //用于获取种子数据
        std::mt19937 gen(rd()); //使用Mersenne Twister算法的随机数生成器
        std::uniform_int_distribution<> dis(44.5, 45.5); //定义一个区间，生成这个区间内的随机数

        int randomValue = dis(gen); //生成随机数
        emit sigSetRobotPose(116-i,randomValue,180); //1
    }
    ui->labelPose->setText("当前位姿：(67,45,180)");
    ui->VelWidget->setValue(17);
}


void frmMain::on_btnRealTimeDetect_clicked()
{
    static bool checked = true;
    checked = !checked;

    if(checked)
    {
        ui->btnRealTimeDetect->setText("开启实时检测");
        ui->btnRealTimeDetect->setIcon(QIcon(":/image/switch-on.png"));
    }
    else
    {
        ui->btnRealTimeDetect->setText("关闭实时检测");
        ui->btnRealTimeDetect->setIcon(QIcon(":/image/switch-off.png"));
    }
    ui->widgetVideo->setEnableDetect(checked);
}


void frmMain::on_btnOpenSerialPort_clicked()
{
    if (ui->btnOpenSerialPort->text() == "打开串口") {
        pTestSerial = new QSerialPort(this);
        pTestSerial->setBaudRate(QSerialPort::Baud115200);
        pTestSerial->setDataBits(QSerialPort::Data8);
        pTestSerial->setStopBits(QSerialPort::OneStop);
        pTestSerial->setParity(QSerialPort::NoParity);
        pTestSerial->setFlowControl(QSerialPort::NoFlowControl);
        connect(pTestSerial, &QSerialPort::readyRead, this, &frmMain::slotTestSerialReadyRead);

        QString port = ui->cBoxPortNumber->currentText();
        pTestSerial->setPortName(port);
        if (!pTestSerial->open(QIODevice::ReadWrite)) {
            pStatusWidget->setShowMessage("警告", pSerial->errorString());
            pStatusWidget->start();
            return;
        }
        ui->btnOpenSerialPort->setText("关闭串口");
        connect(ui->cBoxSerialTimer, &QCheckBox::stateChanged, [this](int state){
            if(state)
            {
                bool ok;
                pSerialTiming = new QTimer;
                QString timStr = ui->SerialPortTiming->text();
                int tim = timStr.toInt(&ok, 10);
                if(ok)
                {
                    connect(pSerialTiming, &QTimer::timeout, this, &frmMain::slotSerialTimingTimeout);
                    pSerialTiming->setInterval(timStr.toInt());
                    pSerialTiming->start();
                }
                else
                {
                    pStatusWidget->setShowMessage("警告", "定时时间必须为整数!");
                    pStatusWidget->start();
                }

            }
            else
            {
                pSerialTiming->stop();
                disconnect(pSerialTiming, &QTimer::timeout, this, &frmMain::slotSerialTimingTimeout);
                if(pSerialTiming != nullptr)
                {
                    delete pSerialTiming;
                    pSerialTiming = nullptr;
                }

            }
        });
    } else {
        if (!pTestSerial->isOpen())
            return;
        pTestSerial->close();
        ui->btnOpenSerialPort->setText("打开串口");
    }
}

void frmMain::slotSerialTimingTimeout()
{
    on_btnSerialSend_1_clicked();
}

void frmMain::on_btnSerialSend_1_clicked()
{
    QString text = ui->lEditSerialSend_1->text();
    QByteArray data;
    if(ui->cBoxHexSend->isChecked())
    {
        QStringList list = text.split(" ");
        for(int i = 0; i < list.size(); i++)
        {
            bool ok;
            int hex = list[i].toInt(&ok, 16);
            if(ok)
            {
                data.append(hex);
            }
        }
    }
    else
    {
        data = text.toLocal8Bit();
    }
    serialTxCount += data.size();
    ui->SerialTxCount->setText(QString::number(serialTxCount));
    pTestSerial->write(data);

    if(ui->cBoxTimestamp->isChecked())
    {
        QString timeStr = QTime::currentTime().toString("HH:mm:ss:zzz");
        text = "<font color=\"#FF00EB\">" + timeStr + " &gt;&gt; </font>" + text;
    }
    if(ui->cBoxShowSendData->isChecked())
    {
        ui->tEditSerialRecv->append(text);
    }
    QThread::msleep(5);
}

void frmMain::slotTestSerialReadyRead()
{
    QByteArray array = pTestSerial->readAll();
    QString rStr;
    if(ui->cBoxHexShow->isChecked())
    {
        rStr = array.toHex(' ').toUpper();
    }
    else
    {
        rStr = QString(array);
    }

    if(ui->cBoxTimestamp->isChecked())
    {
        QString timeStr1 = QTime::currentTime().toString("HH:mm:ss:zzz");

        rStr = "<font color=\"#FF8100\">" + timeStr1 + " &lt;&lt; </font>" + rStr;
    }

    ui->tEditSerialRecv->append(rStr);
    serialRxCount += array.size();
    ui->SerialRxCount->setText(QString::number(serialRxCount));
}

void frmMain::on_btnRefreshPort_2_clicked()
{
    ui->cBoxPortNumber->clear();
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()) /* 初始化串口列表 */
    {
        ui->cBoxPortNumber->addItem(info.portName());
    }
}

void frmMain::on_btnClearSerialRecv_clicked()
{
    ui->tEditSerialRecv->clear();
}

void frmMain::on_btnClearTxCount_clicked()
{
    serialTxCount = 0;
    ui->SerialTxCount->setText(QString::number(serialTxCount));
}


void frmMain::on_btnClearRxCount_clicked()
{
    serialRxCount = 0;
    ui->SerialRxCount->setText(QString::number(serialRxCount));
}

void frmMain::on_tBtnScreenShot_clicked()
{
    QString timeStr1 = QTime::currentTime().toString("HH_mm_ss_zzz");
    QSettings settings(CONFIG_FILEPATH, QSettings::IniFormat);
    QString imageSavePath = settings.value("Config1/imageSavePath").toString();
    QString path = imageSavePath + timeStr1 + ".jpg";
    qDebug() << path;
    ui->widgetVideo->screenShot(path);
}

