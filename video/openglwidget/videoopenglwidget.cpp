#include "videoopenglwidget.h"
#include <QDebug>

extern QQueue<cv::Mat> videoFrameQueue;
extern QMutex videoMutex;

extern QQueue<ROI_FRAME> roiFrameQueue;
extern QMutex roiMutex;

VideoOpenGLWidget::VideoOpenGLWidget(QWidget *parnet, bool enablePython)
    : QOpenGLWidget(parnet)
    , enablePython(enablePython)
    , enableDetect(false)
{
    pVideoThread = new OpencvThread;
    pPythonThread = new PythonThread;
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &VideoOpenGLWidget::updateImage);
    connect(pPythonThread,
            &PythonThread::sigModelLoadSuccess,
            this,
            &VideoOpenGLWidget::slotModelLoadSuccess); /* 加载模型成功后启用检测函数 */
}

VideoOpenGLWidget::~VideoOpenGLWidget()
{
    if (timer->isActive()) {
        timer->stop();
    }
    if (this->getEnablePython()) {
        if (pPythonThread->isRunning()) {
            /* 线程退出之后再进去就会在加载模块的地方失败 */
            pPythonThread->close();
            pPythonThread->quit();
            pPythonThread->wait(500);
            //            pPythonThread->pause();
        }
    }

    if (pVideoThread->isRunning()) {
        pVideoThread->close();
        pVideoThread->quit();
        pVideoThread->wait(500);
    }

    QTimer::singleShot(1, this, &VideoOpenGLWidget::clear);
}

void VideoOpenGLWidget::updateImage()
{
    Mat srcFrame;

    if (this->getEnableDetect()) {
        ROI_FRAME roiFrame;
        roiMutex.lock();
        if (!roiFrameQueue.isEmpty()) {
            roiFrame = roiFrameQueue.dequeue();
            //            qDebug() << "python: " + QString::number(roiFrameQueue.size());
            if (roiFrameQueue.size() > 3)
                roiFrameQueue.clear();
        }
        roiMutex.unlock();
        srcFrame = roiFrame.frame;
    } else {
        videoMutex.lock();
        if (!videoFrameQueue.isEmpty()) {
            srcFrame = videoFrameQueue.dequeue();
            if (videoFrameQueue.size() > 3)
                videoFrameQueue.clear();
        }

        videoMutex.unlock();
    }

    m_img = cvMat2QImage(srcFrame);

    if (m_img.isNull()) {
        qDebug() << "Null Image";
    } else {

        if(bScreenshot)
        {
            m_img.save(mScreenShotPath);
            bScreenshot = false;
        }
        update();

    }
}

void VideoOpenGLWidget::setAddr(const QString Url)
{
    pVideoThread->setAddr(Url);
}

void VideoOpenGLWidget::setAddr(int index)
{
    pVideoThread->setAddr(index);
}

void VideoOpenGLWidget::open()
{
    pVideoThread->open();
    pVideoThread->start();
    QThread::sleep(1);
    if (this->getEnablePython()) {
        pPythonThread->open();
        pPythonThread->start();
    }
    timer->start(50);
}

void VideoOpenGLWidget::clear()
{
    m_img = QImage();
    update();
}

void VideoOpenGLWidget::close()
{
    if(timer->isActive())
    {
        timer->stop();
    }
    if (this->getEnablePython()) {
        if (pPythonThread->isRunning()) {
            /* 线程退出之后再进去就会在加载模块的地方失败 */
            //        thread2->close();
            //        thread2->quit();
            //        thread2->wait(500);
            pPythonThread->pause();
        }
    }

    if (pVideoThread->isRunning()) {
        pVideoThread->close();
        pVideoThread->quit();
        pVideoThread->wait(500);
    }

    QTimer::singleShot(1, this, &VideoOpenGLWidget::clear);
}

void VideoOpenGLWidget::slotModelLoadSuccess()
{
    setEnableDetect(true);
}

QImage VideoOpenGLWidget::cvMat2QImage(const Mat &mat)
{
    if (mat.type() == CV_8UC3) // 8-bits unsigned, NO. OF CHANNELS = 3
    {
        const uchar *pSrc = reinterpret_cast<const uchar *>(mat.data); // Copy input Mat
        QImage image(pSrc,
                     mat.cols,
                     mat.rows,
                     static_cast<int>(mat.step),
                     QImage::Format_RGB888); // Create QImage with same dimensions as input Mat
        return image.rgbSwapped();
    }
    else {
        return QImage(); // Mat could not be converted to QImage
    }
}

bool VideoOpenGLWidget::getEnableDetect() const
{
    return enableDetect;
}

void VideoOpenGLWidget::setEnableDetect(bool newEnableDetect)
{
    if (enableDetect == newEnableDetect)
        return;
    enableDetect = newEnableDetect;
    emit enableDetectChanged();
}

bool VideoOpenGLWidget::getEnablePython() const
{
    return enablePython;
}

void VideoOpenGLWidget::setEnablePython(bool newEnablePython)
{
    enablePython = newEnablePython;
}

void VideoOpenGLWidget::screenShot(const QString &filepath)
{
    bScreenshot = true;
    mScreenShotPath = filepath;
}
