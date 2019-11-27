#include "visualalert.h"

VisualAlert::VisualAlert(System *sys) {
    system = sys;
    close = false;
    connect(this, SIGNAL(finished()), this, SLOT(deleteLater()));
    //connect(this, SIGNAL(rectChanged(QRectF)), sys, SLOT(updateRect(QRectF)));
}

VisualAlert::~VisualAlert() {
    cout << "Destructor visual alert: " << qPrintable(system->getName()) << endl;
}

void    VisualAlert::run() {
    cout << "start Opacity" << endl;

    int duration = 10;
    int tick = 100;

    for (int i = 0; i < tick; i++) {
        if (system == NULL || close == true) {
            cout << "Opacity thread interrupt" << endl;
            return ;
        }
        float coef = (10 - abs(10/2 - i % 10)) / float(10);
        QRectF rect(system->getX() * POS_COEF_X + MARGINX - system->getText()->boundingRect().width() * (coef*1.5) / 2, \
                    system->getY() * POS_COEF_Y + MARGINY - RECT_HEIGHT * (coef*1.5) / 2 + RECT_HEIGHT / 2, \
                    system->getText()->boundingRect().width() * (coef*1.5), \
                    RECT_HEIGHT * (coef*1.5));
        QMetaObject::invokeMethod(system, "updateRectangle", Qt::QueuedConnection, Q_ARG(QRectF, rect), Q_ARG(float, coef));
        QThread::usleep((duration * 1000000) / tick);
    }
    QRectF rect(system->getX() * POS_COEF_X + MARGINX - system->getText()->boundingRect().width() / 2, \
                system->getY() * POS_COEF_Y + MARGINY, \
                system->getText()->boundingRect().width(), \
                RECT_HEIGHT);
    QMetaObject::invokeMethod(system, "updateRectangle", Qt::QueuedConnection, Q_ARG(QRectF, rect), Q_ARG(float, 1.0));
    cout << "end Opacity thread" << endl;
}

void        VisualAlert::close_thread() {
    cout << "Opacity thread manual close" << endl;
    this->close = true;
}
