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
        None,
        Done,
        Undone
    };

    GoalItem(QGraphicsObject *parent = nullptr);
    GoalItem(int id = 0, QGraphicsObject *parent = nullptr);

    QRectF boundingRect() const ;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,QWidget *widget);

    void setGoalType(GOALTYPE type);
    void setGoalID(int id);
    int  getGoalID() const;
private:

    const int width = 6;
    const int height = 6;
    const int width_2 = width/2;
    const int height_2 = height/2;

    GOALTYPE m_Type;
    int m_ID;
};


#endif // ROBOT_H
