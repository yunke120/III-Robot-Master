#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>

#define INIT 1

#if INIT
// video
//#include "video/widget/videowidget.h"

// statuswidget
#include "statuswidget/statuswidget.h"

#include "sql/sql.h"

#include "protocol/protocol.h"

#include "dashboard/dashboard.h"
#include "map/robot.h"
#include "log4qt/log.h"
#define CONFIG_FILEPATH "./config.ini"
#endif

#define IMAGE_HEIGHT   100

class QAbstractButton;

namespace Ui {
class frmMain;
}

class frmMain : public QWidget
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = nullptr);
    ~frmMain();
        QStringList taskList;
private:
    bool createFolder(const QString& folder);
protected:
    bool eventFilter(QObject *watched, QEvent *event);

private:
    Ui::frmMain *ui;

    QList<int> iconsMain;
    QList<QAbstractButton *> btnsMain;

    QList<int> iconsConfig;
    QList<QAbstractButton *> btnsConfig;
#if INIT
    QSerialPort *pSerial;                       /* 串口对象 */
    StatusWidget *pStatusWidget;
    LoggerSql *plogSql;

    QTimer *pDataTimer; /**/
    QQueue<QByteArray> mRecvQueue;
    QByteArray AllArray; /* 串口接收数据缓存 */



#endif
private:
    //根据QSS样式获取对应颜色值
    QString borderColor;
    QString normalBgColor;
    QString darkBgColor;
    QString normalTextColor;
    QString darkTextColor;

    void getQssColor(const QString &qss, const QString &flag, QString &color);
    void getQssColor(const QString &qss, QString &textColor,
                     QString &panelColor, QString &borderColor,
                     QString &normalColorStart, QString &normalColorEnd,
                     QString &darkColorStart, QString &darkColorEnd,
                     QString &highColor);
#if INIT
    void appendDatat2LogWidget(const QList<QVariantMap> &data);
    void appendData2ReportWidget(const QString &data1, const QImage &image);
    uint8_t checkNumber(const char *data, unsigned char len);
    void _sendCommand(eDEVICE device, unsigned char cmd);
    void _sendCommand(eDEVICE device, unsigned char cmd, unsigned char *userdata, unsigned char len);
#endif
private slots:
    void initForm();
    void initStyle();
    void buttonClick();
    void initLeftMain();
    void initLeftConfig();
    void leftMainClick();
    void leftConfigClick();
#if INIT
    void initSerialPort();
    void initConfig();
    void initLogSql();
    void initMap();
    void initReport();
    void initHelp();
#endif
private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();
#if INIT
    void slotSerialReadyRead();
    void slotDataTimerOut(void);
    void slotConfigChange(int index);
    void on_btnRobotUp_clicked();
    void on_btnRobotDown_clicked();
    void on_btnRobotLeft_clicked();
    void on_btnRobotRight_clicked();
    void on_btnRobotStop_clicked();
    void on_btnSaveConfig_clicked();
    void on_btnSaveImage_clicked();
    void on_btnC1Cancel_clicked();
    void on_btnSaveVideo_clicked();
    void on_btnC1Apply_clicked();
    void on_btnC1Confirm_clicked();
    void on_btnC2Cancel_clicked();
    void on_btnC2Confirm_clicked();
    void on_btnRefreshPort_clicked();

    void on_btnOpenSerial_clicked();
    void on_tbFilter_clicked();
    void on_tbSelectLogFile_clicked();
    void on_btnRobotLeftUp_clicked();
    void on_btnRobotRightUp_clicked();
    void on_btnRobotLeftDown_clicked();
    void on_btnRobotRightDown_clicked();
    // void on_btnRobotAccelerate_clicked();
    // void on_btnDecelerate_clicked();
    void on_btnGPU_clicked();
    void on_btnOpenVideo_clicked();
    void on_btnImportMap_clicked();
    void on_btnAddGoalPoint_clicked();
    void on_btnSaveTaskList_clicked();
    void on_btnImportTaskList_clicked();
    void on_btnExportReport_clicked();
#endif


    void on_btnRealTimeDetect_clicked(bool checked);

signals:
    void sig_ImportMap(const QString &filepath);
    void sigSetRobotPose(int x, int y, int w);
    void sigSetGoalStatus(int id, GoalItem::GOALTYPE type);
};

#endif // FRMMAIN_H
