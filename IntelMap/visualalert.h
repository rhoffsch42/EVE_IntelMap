#ifndef VISUALALERT_H
#define VISUALALERT_H

#include <QApplication>
#include <QThread>
#include <QGraphicsRectItem>
#include "system.h"

class System;

class VisualAlert : public QThread
{
    Q_OBJECT

public:
    VisualAlert(System *sys);
    ~VisualAlert();
    System  *system;

signals:
    void    rectangleChanged(int);

public slots:
    void        close_thread();

protected:
    void    run() Q_DECL_OVERRIDE;

private:
    bool    close;
};

#endif // VISUALALERT_H
