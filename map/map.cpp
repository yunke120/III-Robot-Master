#include "map.h"

const QPointF g_GoalPoint[26] = {};


/* 显示地图界面
 * 接收一个信号传递图片路径
 */

MapWidget::MapWidget(QWidget *parent) : QGraphicsView(parent), IsMove(false)
{
    this->setTransformationAnchor(QGraphicsView::AnchorUnderMouse); // scaled变换时以鼠标为中心变换
    pScene = new MapSence(this);
    // pScene->setSceneRect(-viewport()->width()/2,-viewport()->height()/2,viewport()->width()*2, viewport()->height()*2);
    // pScene->setBackgroundBrush(Qt::gray);
    this->setScene(pScene);

    pixmapItem = new QGraphicsPixmapItem();
    pixmapItem->setZValue(0);
    pScene->addItem(pixmapItem);

    pathItem = new QGraphicsPathItem(robotPath);
    QPen pen = pathItem->pen();
    pen.setWidth(2);
    pen.setColor(Qt::red);
    pathItem->setPen(pen);
    pScene->addItem(pathItem);

    goalPointList << QPointF(120,180) << QPointF(100,180) << QPointF(80,180) << QPointF(60,180) << QPointF(40,180)
    << QPointF(40,140) << QPointF(60,140) << QPointF(80,140) << QPointF(100,140) << QPointF(120,140) << QPointF(140,140) << QPointF(160,140)
    << QPointF(160,100) << QPointF(140,100) << QPointF(120,100) << QPointF(100,100) << QPointF(80,100) << QPointF(60,100) << QPointF(40,100)
    << QPointF(40,60) << QPointF(60,60) << QPointF(80,60) << QPointF(100,60) << QPointF(120,60) << QPointF(140,60) << QPointF(160,60);
}

MapWidget::~MapWidget()
{
    delete pixmapItem;
    pixmapItem = nullptr;
}

void MapWidget::wheelEvent(QWheelEvent *event)
{
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    qreal scaleFactor = (event->angleDelta().y() > 0) ? 1.05 : 0.95;
    scale(scaleFactor, scaleFactor);
#else
    event->delta() > 0 ? scale(1.05,1.05) : scale(0.95,0.95);
#endif
}

void MapWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(IsMove)
    {
        this->setCursor(Qt::ClosedHandCursor);
        QPointF posDelta = event->pos() - mLastPos;

        int w = viewport()->rect().width();
        int h = viewport()->rect().height();
        QPoint newCenter(w / 2. - posDelta.x()+0.5,  h / 2. - posDelta.y()+0.5);
        centerOn(mapToScene(newCenter));
        mLastPos = event->pos();
    }

    QGraphicsView::mouseMoveEvent(event);
}

void MapWidget::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton) {
        mLastPos = event->pos();
        IsMove = true;
    }
}

void MapWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event)
    IsMove = false;
    this->setCursor(Qt::ArrowCursor);
}

/**
 * @brief MapWidget::showMap
 * @param filepath:图片文件路径
 */
void MapWidget::showMap(const QString &filepath)
{
    QPixmap pixmap(filepath);
    int _w = pixmap.width()-200;
    int _h = pixmap.height()-200;
    QPixmap rpixmap = pixmap.transformed(QTransform().rotate(180));
    pixmapItem->setPos(-_w/2,-_h/2);
    pixmapItem->setPixmap(rpixmap);

    robotItem = new RobotItem;
    robotItem->setPos(160,180);
    robotItem->setZValue(1);
    robotItem->setAngle(180);
    pScene->addItem(robotItem);
    robotPath.moveTo(160,180);

    goalItemMap.clear();
    for(int id = 1; id <= 26; id++)
    {
        GoalItem *item = new GoalItem(id);
        item->setZValue(1);
        item->setPos(goalPointList.at(id-1));
        pScene->addItem(item);
        goalItemMap.insert(id, item);
    }
}

void MapWidget::slotSetRobotPose(int x, int y, int w)
{
    robotItem->setPos(x, y);
    robotItem->setAngle(w);
    robotPath.lineTo(x,y);
    pathItem->setPath(robotPath);
}

void MapWidget::slotSetGoalStatus(int id, GoalItem::GOALTYPE type)
{
    goalItemMap.value(id)->setGoalType(type);
    goalItemMap.value(id)->update();
    // slotSetRobotPose(goalPointList.at(id).x(),goalPointList.at(id).y(), 180);
}

MapSence::MapSence(QObject *parent): QGraphicsScene(parent)
{
    this->setBackgroundBrush(QColor(QRgb(0x484848)));
}

MapSence::~MapSence()
{

}

void MapSence::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void MapSence::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void MapSence::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);
    // 设置网格的间隔和颜色
    qreal gridSize = 20;
    QColor gridColor(135, 135, 135);

    // 根据网格间隔计算绘制的起点
    qreal left = int(rect.left() / gridSize) * gridSize;
    qreal top = int(rect.top() / gridSize) * gridSize;

    // 绘制垂直线
    for (qreal x = left; x < rect.right(); x += gridSize)
    {
        painter->setPen(gridColor);
        painter->drawLine(QPointF(x, rect.top()), QPointF(x, rect.bottom()));
    }

    // 绘制水平线
    for (qreal y = top; y < rect.bottom(); y += gridSize)
    {
        painter->setPen(gridColor);
        painter->drawLine(QPointF(rect.left(), y), QPointF(rect.right(), y));
    }
}
