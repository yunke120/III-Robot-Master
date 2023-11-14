
#include "opencvthread.h"
#include <QDebug>
QQueue<cv::Mat> videoFrameQueue;
QMutex videoMutex;

// opencv编译 https://zhuanlan.zhihu.com/p/594507922
OpencvThread::OpencvThread(QObject *parent) : QThread (parent)
{
    setObjectName("OpencvThread");
    stopped = false;
}

void OpencvThread::run()
{
    QElapsedTimer ElapsedTimer;
    ElapsedTimer.start();
    qDebug() << "opencv entry";
    cv::VideoCapture pCap;
    // https://blog.csdn.net/weixin_43272781/article/details/103787735
    if (addrType)
        //        pCap = new VideoCapture(Url.toStdString());
        //        cv::VideoCapture pCap(Url.toStdString());
        pCap.open(Url.toStdString());
    else
        //        pCap = new VideoCapture(Addr);
        //        cv::VideoCapture pCap(Addr);
        pCap.open(Addr);

    if (!pCap.isOpened())
    //    if(pCap == nullptr || !pCap->isOpened())
    {
        emit sigReset();
        qDebug() << "opencv falied";
        return;
    }

    int timeout = 3;
    qDebug() << "耗时" << ElapsedTimer.elapsed() << "毫秒";
    qDebug() << "opencv start";
    while(stopped)
    {
        Mat frame;
        pCap >> frame;
        if(frame.empty())
        {
            timeout --;
            if(timeout == 0)
                break;
            continue;
        } else {
            timeout = 3;
            videoMutex.lock();
            videoFrameQueue.enqueue(frame);
            //            qDebug() << "opencv: " << videoFrameQueue.size();
            videoMutex.unlock();
        }

        msleep(10);
    }
    if(timeout == 0)
    {
        emit sigReset();
    }
    stopped = false;
    //    pCap->release();
    pCap.release();
    //    delete pCap;
    //    pCap = nullptr;
    return;
}

void OpencvThread::setAddr(const QString url)
{
    this->Url = url;
    addrType = true;
}

void OpencvThread::setAddr(int index)
{
    this->Addr = index;
    addrType = false;
}

void OpencvThread::open()
{
    QMutexLocker locker(&mutex);
    stopped = true;
}

void OpencvThread::close()
{
    QMutexLocker locker(&mutex);
    stopped = false;
}
