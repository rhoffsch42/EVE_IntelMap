#ifndef SCENE_H
#define SCENE_H

#include "data.h"
#include "link.h"
#include "system.h"
#include "visualalert.h"
#include <QApplication>
#include <QString>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QImage>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QDir>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>

class System;
class Link;
class VisualAlert;

class Scene : public QWidget
{
    Q_OBJECT

    public:
    Scene(QString scene_name, t_reg *region_data);
    ~Scene();
    QString         getName();
    QGraphicsView   *getView();
    System          *getSystem(int id);
    t_reg           *getRegionData();
    void            setImage(QString image_path);
    void            drawSystems();
    void            CloseThreads();

    QGraphicsPixmapItem     *bg_item;
    QGraphicsScene          *scene;

signals:
    void        alert_audio();
    void        changingRegion();

public slots:
    void        alert_system(int sys_name);

private:
    QString                 name;
    list<System*>           system_list;
    list<Link*>             link_list;
    t_reg                   *region_data;
    QGraphicsView           *view;
    list<VisualAlert*>      thread_list;
};

#endif // SCENE_H
