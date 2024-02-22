

#ifndef __MAP_H__
#define __MAP_H__

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QGraphicsSimpleTextItem>
#include <QWheelEvent>
#include <QPixmap>


#include "robot.h"
class MapSence;
class MapWidget : public QGraphicsView
{
    Q_OBJECT
public:
    MapWidget(QWidget *parent = nullptr);
    ~MapWidget();

protected:

    virtual void wheelEvent(QWheelEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event) ;
    virtual void mouseReleaseEvent(QMouseEvent *event) ;

public slots:
    void showMap(const QString &filepath);
    void slotSetRobotPose(int x, int y, int w);
    // void slotAddGoalPoint(QPointF);
    void slotSetGoalStatus(int id, GoalItem::GOALTYPE type);
private:
    MapSence *pScene;
    QGraphicsPixmapItem *pixmapItem;
    QPoint mLastPos;
    bool IsMove;

    RobotItem *robotItem;
    QGraphicsPathItem *pathItem;
    QPainterPath robotPath;

    QMap<int, GoalItem*> goalItemMap;
    QList<QPointF> goalPointList;
};



class MapSence : public QGraphicsScene
{
public:
    explicit MapSence(QObject *parent = nullptr);
    ~MapSence();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent);
    void drawBackground(QPainter *painter, const QRectF &rect);
};




#endif
