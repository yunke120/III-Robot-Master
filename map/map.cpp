#include "map.h"

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
    pathItem->setBrush(QBrush(QColor(0, 160, 230)));
    pScene->addItem(pathItem);
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
    QPixmap rpixmap = pixmap.transformed(QTransform().rotate(180));
    pixmapItem->setPos(0,0);
    pixmapItem->setPixmap(rpixmap);

    robotItem = new RobotItem;
    robotItem->setPos(100,100);
    robotItem->setZValue(1);
    pScene->addItem(robotItem);
    robotPath.moveTo(0,0);
}

void MapWidget::slotSetRobotPose(int x, int y, int w)
{
    robotItem->setPos(x, y);
    robotItem->setAngle(w);
    robotPath.lineTo(x,y);
    // pathItem->update();
    pathItem->setPath(robotPath);

    QPointF p(x+10, y+10);
    slotAddGoalPoint(p);
}

void MapWidget::slotAddGoalPoint(QPointF point)
{
    GoalItem *item =  new GoalItem;
    item->setZValue(0);
    item->setPos(point);
    pScene->addItem(item);
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
