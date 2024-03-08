#ifndef VIDEOOPENGLWIDGET_H
#define VIDEOOPENGLWIDGET_H

#include <QImage>
#include <QPainter>
#include <QTimer>

#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include "../opencv2/OpencvThread.h"
#include "../python/pythonthread.h"

class VideoOpenGLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

    Q_PROPERTY(bool enablePython READ getEnablePython WRITE setEnablePython FINAL)
    Q_PROPERTY(bool enableDetect READ getEnableDetect WRITE setEnableDetect NOTIFY
                   enableDetectChanged FINAL)
public:
    VideoOpenGLWidget(QWidget *parnet = nullptr, bool enablePython = false);
    ~VideoOpenGLWidget();
    inline void setImage(const QImage &img){
        m_img = img;
        this->update();
    }

    void setAddr(const QString Url);
    void setAddr(int index);

    bool getEnableDetect() const;
    void setEnableDetect(bool newEnableDetect);

    bool getEnablePython() const;
    void setEnablePython(bool newEnablePython);

    void screenShot(const QString &filepath);

private:
    QImage m_img;
    OpencvThread *pVideoThread;
    PythonThread *pPythonThread;
    QTimer *timer;

    bool enablePython; /* 启用Python算法 */
    bool enableDetect; /* 启用算法检测 */

    bool bScreenshot = false;
    QString mScreenShotPath;
signals:

    void enableDetectChanged();

protected:
    void paintEvent(QPaintEvent *)
    {
        QPainter p(this);
        p.setRenderHint(QPainter::SmoothPixmapTransform, 1);
        p.drawImage(this->rect(), m_img);
    }
private slots:
    void updateImage(void);
    QImage cvMat2QImage(const Mat &mat);

public slots:
    void open();
    void clear();
    void close();

    void slotModelLoadSuccess();
};

#endif /* VIDEOOPENGLWIDGET_H */
