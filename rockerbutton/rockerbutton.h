#ifndef ROCKERBUTTON_H
#define ROCKERBUTTON_H

#include <QWidget>
#include <QPoint>
#include "qmath.h"
#include <QMouseEvent>

#define PI   3.1415926

class RockerButton : public QWidget
{
    Q_OBJECT
public:
    explicit RockerButton(QWidget *parent = nullptr);

public:

    void     setOuterCircleRadius(int radius);
    void     setInnerCircleRadius(int radius);
    void     setStyleSheet(QString);

signals:
    void     sig_MouseMovePolar(int, int);
    void     sig_MouseMoveRectangular(int,int);
    void     sig_MouseClicked(void);
    void     sig_MouseReleased(void);

protected:
    void     paintEvent(QPaintEvent *);
    void     mouseMoveEvent(QMouseEvent *event);
    void     mousePressEvent(QMouseEvent *event);
    void     mouseReleaseEvent(QMouseEvent *event);
private:

    void     gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor color);
    QPixmap  getSignalPixmap(const QColor &color, const int linenum);
    int      getLineNum(const int ping);
    double   getAngle(int x,int y);

private:
    QPixmap  mPixmap;
    int      m_radius, m_arcLength;
    QPoint   mCenterRound;

    QColor   mInnerCircleColor;
    QColor   mOuterCircleColor;

    QColor   mLineColor;

    QColor   colorExcircle0 = QColor(68, 68, 68);
    QColor   colorExcircle5 = QColor(37, 40, 46);
    QColor   colorExcircle9 = QColor(22, 22, 22);


    bool     m_isMousePressed;

    int      angle;
    int      offset;

};

#endif // ROCKERBUTTON_H
