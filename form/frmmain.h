#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>


// video
//#include "video/widget/videowidget.h"

// statuswidget
#include "statuswidget/statuswidget.h"

#include "sql/sql.h"

#include "protocol/protocol.h"

#include "dashboard/dashboard.h"

#define CONFIG_FILEPATH "./config.ini"

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

    QSerialPort *pSerial;                       /* 串口对象 */
    StatusWidget *pStatusWidget;
    LoggerSql *plogSql;

    QTimer *pDataTimer; /**/
    QQueue<QByteArray> mRecvQueue;
    QByteArray AllArray; /* 串口接收数据缓存 */

    int mDefaultVelocity; /* 使用double会出现浮点数的精度问题 */

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

    void appendDatat2LogWidget(const QList<QVariantMap> &data);
    uint8_t checkNumber(const char *data, unsigned char len);
    void _sendCommand(eDEVICE device, unsigned char cmd);
    void _sendCommand(eDEVICE device, unsigned char cmd, unsigned char *userdata, unsigned char len);
private slots:
    void initForm();
    void initStyle();
    void buttonClick();
    void initLeftMain();
    void initLeftConfig();
    void leftMainClick();
    void leftConfigClick();
    void initSerialPort();
    void initConfig();
    void initLogSql();

private slots:
    void on_btnMenu_Min_clicked();
    void on_btnMenu_Max_clicked();
    void on_btnMenu_Close_clicked();

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
    void on_btnRobotAccelerate_clicked();
    void on_btnDecelerate_clicked();
    void on_btnGPU_clicked();
    void on_btnOpenVideo_clicked();
};

#endif // FRMMAIN_H
