#ifndef SYSTEM_H
#define SYSTEM_H

#include "data.h"
#include "scene.h"
#include <QApplication>
#include <QWidget>
#include <QString>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <string>
#include <QGraphicsSceneMouseEvent>


#include <QGraphicsDropShadowEffect>
#include "visualalert.h"

class Scene;

class System : public QObject, public QGraphicsRectItem
{
    Q_OBJECT

public:
    System(t_sys *system, Scene *scene);
    t_sys               *getSysData();
    QColor              getColor();
    QGraphicsTextItem   *getText();
    QString             getName();
    int                 getID();
    int                 getX();
    int                 getY();
    Scene               *scene_ptr;
    Q_INVOKABLE void    updateRectangle(QRectF rectangle, float opacity);

protected:
    void    mousePressEvent(QGraphicsSceneMouseEvent *event);

private:
    t_sys               *sys_data;
    QString             name;
    int                 id;
    int                 x;
    int                 y;
    QColor              sys_color;
    QColor              rec_color;
    QColor              text_color;
    QGraphicsTextItem   *text;
};

#endif // SYSTEM_H
