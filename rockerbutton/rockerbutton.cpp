#include "rockerbutton.h"
#include <QPainter>
#include <QPainterPath>
#include <QTransform>
RockerButton::RockerButton(QWidget *parent)
    : QWidget(parent)
    , m_radius(80)
    , m_arcLength(55)
    , mCenterRound(QPoint(0, 0))
    , mInnerCircleColor(QColor(255,0,0,0))
    , mOuterCircleColor(QColor(38,38,38))
    , mLineColor(QColor(5,199,199))
    , m_isMousePressed(false)
{
//    this->setMouseTracking(false);
}

void RockerButton::setOuterCircleRadius(int radius)
{
     m_radius = radius;
     update();
}

void RockerButton::setInnerCircleRadius(int radius)
{
    Q_ASSERT(radius >= m_radius);
    m_arcLength = m_radius - radius;
    update();
}

void RockerButton::setStyleSheet(QString style)
{
    if(style == "Dark")
    {
//        mInnerCircleColor = (QColor(255,0,0,0));
        mOuterCircleColor = QColor(38,38,38);
        mLineColor = QColor(5,199,199);
        colorExcircle0 = QColor(68, 68, 68);
        colorExcircle5 = QColor(37, 40, 46);
        colorExcircle9 = QColor(22, 22, 22);
    }
    else if(style == "light"){
//        mInnerCircleColor = (QColor(255,0,0,0));
        mOuterCircleColor = QColor(18,150,219);
        mLineColor = QColor(255,0,255);
        colorExcircle0 = QColor(136,136,255);
        colorExcircle5 = QColor(119,119,255);
        colorExcircle9 = QColor(102,102,255);
    }
    else {

    }
}

void RockerButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.translate(width() >> 1, height() >> 1); // 将图像画在中心

    gradientArc(&painter, m_radius, 0, 360, m_arcLength, mOuterCircleColor); /* 无级调节 */

    QLinearGradient  linearGradient = QLinearGradient(0, mCenterRound.y() - m_radius + m_arcLength - 1, 0, mCenterRound.y()+m_radius - m_arcLength + 1);
    linearGradient.setColorAt(0.0, colorExcircle0);
    linearGradient.setColorAt(0.5, colorExcircle5);
    linearGradient.setColorAt(0.9, colorExcircle9);

    painter.setBrush(QBrush(linearGradient));
    painter.drawEllipse(mCenterRound, m_radius - m_arcLength , m_radius - m_arcLength );

    painter.save();
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    QTransform leftmatrix;
    leftmatrix.rotate(90);
    QRect rect = QRect(30,-30,30,60);
    mPixmap=mPixmap.transformed(leftmatrix,Qt::SmoothTransformation);
    painter.rotate(-angle);
    painter.drawPixmap(rect,mPixmap);
    painter.restore();

}

void RockerButton::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mousePressPoint = event->pos();
    mCenterRound = mousePressPoint - QPoint(width() >> 1, height() >> 1);

    int x = mCenterRound.x();
    int y = mCenterRound.y();

    double offset_d = qSqrt(qPow(x,2)+qPow(y,2)); // pos距中心的偏移量
    double angle_d = getAngle(x, y);              // pos的角度
    // 以此构成极坐标系，角度换算成方向，偏移量换算成速度大小

    double zx = offset_d*cos(angle_d*PI/180.0);
    double zy = offset_d*sin(angle_d*PI/180.0);

    offset = int(offset_d);
    angle = int(angle_d);
    x = int(zx);
    y = int(zy);

    mPixmap = getSignalPixmap(mLineColor, getLineNum(offset));
//    emit sig_MouseMovePolar(offset, angle);
    emit sig_MouseMoveRectangular(x, y);
    update();
}

void RockerButton::mousePressEvent(QMouseEvent *)
{
    m_isMousePressed = true;
//    emit sig_MouseClicked();
    update();
}

void RockerButton::mouseReleaseEvent(QMouseEvent *)
{
    if(!m_isMousePressed)
        return;

    m_isMousePressed = false;
    mCenterRound = QPoint(0,0);
    mPixmap = QPixmap(0,0);
//    emit sig_MouseReleased();
    update();
}


/**
 * 参数二：半径
 * 参数三：开始的角度
 * 参数四：指扫取的角度-顺时针（360度 / 8 = 45度）
 * 参数五：圆环的高度
 * 参数六：填充色
**/
void RockerButton::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QColor color)
{
    // 渐变色
    QRadialGradient gradient(0, 0, radius);
    gradient.setColorAt(0, Qt::white);
    gradient.setColorAt(1.0, color);
    painter->setBrush(gradient);

    QRectF rect(-radius, -radius, radius << 1, radius << 1);
    QPainterPath path;
    path.arcTo(rect, startAngle, angleLength);

    QPainterPath subPath;
    subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

    // path为扇形 subPath为椭圆
    path -= subPath;

    painter->setPen(Qt::NoPen);
    painter->drawPath(path);
}

QPixmap RockerButton::getSignalPixmap(const QColor &color, const int linenum)
{
    QPixmap pixmap(60,30);
    pixmap.fill(mInnerCircleColor);

    QPainter painter(&pixmap);
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.setPen(QPen(color,2,Qt::SolidLine,Qt::RoundCap,Qt::RoundJoin));

    //30 - i* 5 ，是为了顶部不要被削掉
    for(int i=1; i <= linenum; ++i)
    {
        painter.drawArc(30 - i * 6, 30 - i * 5, i * 12, i * 10, 53 * 16, 37 * 2 * 16);
    }
    return pixmap;

}

int RockerButton::getLineNum(const int ping)
{
    if(ping<=20)
        return 1;
    else if(ping<=40)
        return 2;
    else if(ping<=60)
        return 3;
    else if(ping<=80)
        return 4;
    else
        return 5;
}

double RockerButton::getAngle(int x, int y)
{
// 弧度=角度*Pi/180
// 角度 = 弧度*180/PI

    double angle;

    if (x > 0 && y <= 0)
    {
        /* 第一象限 */
        angle = atan(abs((double(y)/x))) * 180 / 3.14159;
    }
    else if(x <= 0 && y < 0)
    {
        /* 第二象限 */
        angle = atan(abs((double(x)/y))) * 180 / 3.14159 + 90.0;
    }
    else if(x < 0 && y >= 0)
    {
        /* 第三象限 */
        angle = atan(abs((double(y)/x))) * 180 / 3.14159 + 180.0;

    }
    else {
        /* 第四象限 */
        angle = atan(abs((double(x)/y))) * 180 / 3.14159 + 270.0;
    }

    return angle;

}
