#include "system.h"

System::System(t_sys *system, Scene *scene) {
    string  currentRegion(qPrintable(scene->getName()));
    std::transform(currentRegion.begin(), currentRegion.end(), currentRegion.begin(), ::tolower);

    sys_data = system;
    name = system->name.c_str();
    id = system->id;
    x = system->x;
    y = system->y;
    scene_ptr = scene;

    sys_color = QColor(HIGHTSEC_COLOR);
    if (system->security_level <= 0)
        sys_color = QColor(NULLSEC_COLOR);
    else if (system->security_level < 0.5)
        sys_color = QColor(LOWSEC_COLOR);
    rec_color = RECT_COLOR;
    text_color = Qt::lightGray;
    if (system->region_belonging->name.compare(currentRegion) != 0) {
        rec_color = Qt::darkGray;
        text_color = Qt::white;
    }


    QPen    pen(sys_color, RECT_LINE_PX);
    this->setPen(pen);
    this->setZValue(1);
    text = new QGraphicsTextItem(name);
    this->setRect(x * POS_COEF_X + MARGINX - text->boundingRect().width() / 2, y * POS_COEF_Y + MARGINY, text->boundingRect().width(), RECT_HEIGHT);
    text->setPos(x * POS_COEF_X + MARGINX - text->boundingRect().width() / 2, y * POS_COEF_Y + MARGINY);
    text->setZValue(2);

    if (sys_data->isTracked == false) {
        text->setDefaultTextColor(text_color);
        this->setBrush(rec_color);
    } else {
        this->setBrush(QBrush(sys_color));
        text->setDefaultTextColor(Qt::black);
        if (sys_color == QColor(NULLSEC_COLOR))
            text->setDefaultTextColor(Qt::yellow);
    }
    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(8);
    effect->setOffset(0, 0);
    effect->setColor(Qt::black);
    this->setGraphicsEffect(effect);

    scene_ptr->scene->addItem(this);
    scene_ptr->scene->addItem(text);

}

void    System::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    cout << "Custom Rect click on " << qPrintable(name) << endl;
    if (event->button() == Qt::LeftButton) {
        if (sys_data->isTracked == false) {
            sys_data->isTracked = true;
            this->setBrush(QBrush(sys_color));
            text->setDefaultTextColor(Qt::black);
            if (sys_color == QColor(NULLSEC_COLOR))
                text->setDefaultTextColor(Qt::yellow);
        } else {
            sys_data->isTracked = false;
            this->setBrush(QBrush(rec_color));
            text->setDefaultTextColor(text_color);
        }
        scene_ptr->getView()->viewport()->update();
    }
}

void    System::updateRectangle(QRectF rectangle, float opacity) {
    //cout << "updating Rectangle" << endl;
    this->setRect(rectangle);
    this->setOpacity(opacity);
    this->scene_ptr->getView()->viewport()->update();
}

t_sys*              System::getSysData(){   return (sys_data);  }
QColor              System::getColor()  {   return (sys_color); }
QGraphicsTextItem*  System::getText()   {   return (text);      }
QString             System::getName()   {   return (name);      }
int                 System::getID()     {   return (id);        }
int                 System::getX()      {   return (x);         }
int                 System::getY()      {   return (y);         }
