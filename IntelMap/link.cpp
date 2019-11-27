#include <link.h>


Link::Link(System *sys1, System *sys2, QGraphicsScene *scene) {
    system1 = sys1;
    system2 = sys2;
    //cout << qPrintable(system1->getName()) << " - " << qPrintable(system2->getName()) << endl;

    QGraphicsLineItem *line;
    QPen    pen(Qt::white, 1, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    line = scene->addLine(system1->getX() * POS_COEF_X + MARGINX, system1->getY() * POS_COEF_Y + MARGINY + sys1->getText()->boundingRect().height() / 2, \
                          system2->getX() * POS_COEF_X + MARGINX, system2->getY() * POS_COEF_Y + MARGINY + sys2->getText()->boundingRect().height() / 2, \
                          pen);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(5);
    effect->setOffset(0, 0);
    effect->setColor(Qt::blue);
    line->setGraphicsEffect(effect);
}

Link::~Link() {
}

void    Link::paintEvent(QPaintEvent *) {
    cout << "painting" << endl;
    QPainter p(this);


    // This is the important part you'll want to play with
    p.setCompositionMode(QPainter::RasterOp_SourceAndNotDestination);
    QPen inverter(Qt::white);
    inverter.setWidth(10);
    p.setPen(inverter);
    p.drawLine(0, 0, 90, 90);
}
