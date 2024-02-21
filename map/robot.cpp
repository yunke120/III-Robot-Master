#include "map/robot.h"
#include <QPainter>


RobotItem::RobotItem(QGraphicsObject *parent)
    : QGraphicsObject(parent)
    , angle(0)
{

}

QRectF RobotItem::boundingRect() const
{
    return QRectF(-width_2,-height_2,width,height);
}

void RobotItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing, true);

    // 定义三角形的三个顶点
    QPointF points[4] = {
        QPointF(-width_2>>2, 0),
        QPointF(-width_2, -height_2),
        QPointF(width_2, 0),
        QPointF(-width_2, height_2),
    };

    // 绘制第一个三角形，通过圆弧替代 path1.lineTo(points[0])
    QPainterPath path1;
    path1.moveTo(points[0]);
    path1.lineTo(points[1]);
    path1.lineTo(points[2]);
    path1.lineTo(points[3]);
    path1.closeSubpath();

    painter->setBrush(QBrush(Qt::darkGreen));
    painter->setPen(Qt::darkGreen);
    painter->drawPath(path1);

    this->setRotation(angle);
}

void RobotItem::setAngle(int w)
{
    angle = w;
    update();
}



GoalItem::GoalItem(QGraphicsObject *parent)
    : QGraphicsObject(parent)
    , m_Type(Undone)
{

}

QRectF GoalItem::boundingRect() const
{
    return QRectF(-width_2,-height_2,width,height);
}

void GoalItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option); Q_UNUSED(widget);

    painter->setRenderHint(QPainter::Antialiasing, true);

    if(m_Type == Undone)
    {
        painter->setPen(QColor(255,0,0));
        painter->setBrush(QBrush(QColor(255,0,0)));
    }
    else if(m_Type == Done)
    {
        painter->setPen(QColor(0,255,0));
        painter->setBrush(QBrush(QColor(0,255,0)));
    }

    painter->drawEllipse(this->boundingRect());
}

void GoalItem::slotSetGoalType(GOALTYPE type)
{
    m_Type = type;
}
