#ifndef ROBOT_H
#define ROBOT_H

#include <QObject>
#include <QGraphicsObject>

#ifndef PI
#define PI	3.141592653
#endif

#define Rad2Degree	360/2/PI


class RobotItem : public QGraphicsObject
{
    Q_OBJECT
public:
    explicit RobotItem(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

private:
    const int width = 12;
    const int height = 12;
    const int width_2 = width/2;
    const int height_2 = height/2;

    int angle;

public slots:
    void setAngle(int w);
};



class GoalItem : public QGraphicsObject
{
    Q_OBJECT
public:

    enum GOALTYPE{
        Done,
        Undone
    };

    explicit GoalItem(QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

private:

    const int width = 6;
    const int height = 6;
    const int width_2 = width/2;
    const int height_2 = height/2;

    GOALTYPE m_Type;

public slots:
    void slotSetGoalType(GOALTYPE type);
};


#endif // ROBOT_H
