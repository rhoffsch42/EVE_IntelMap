#ifndef LINK_H
#define LINK_H

#include "data.h"
#include "system.h"
#include <QApplication>
#include <QWidget>
#include <QGraphicsScene>
#include <iostream>
#include <string>

class System;

class Link : public QWidget
{
    public:
    Link(System *sys1, System *sys2, QGraphicsScene *scene);
    ~Link();

protected:
    void paintEvent(QPaintEvent *);

    private:
    System      *system1;
    System      *system2;
};

#endif // LINK_H
