#include <scene.h>

Scene::Scene(QString scene_name, t_reg *regiondata) : QWidget()
{
    name = scene_name;
    region_data = regiondata;
    scene = new QGraphicsScene();
    scene->addText(name);
    view = new QGraphicsView(scene);
    setImage(BG_PATH + scene_name + ".png");

    QGraphicsDropShadowEffect *effectView = new QGraphicsDropShadowEffect();
    effectView->setBlurRadius(BLUR_RADIUS);
    effectView->setOffset(1,1);
    effectView->setColor(BLUR_COLOR);
    view->setGraphicsEffect(effectView);


    drawSystems();
    //view->show();
    if (DEBUG_METHODS == 1)
        cout << "----------DEBUG scene created" << endl;
}

Scene::~Scene() {
    scene->clear();
    delete view;
    delete scene;
    //delete bg_item;
}

QGraphicsView*  Scene::getView()        {   return(view);           }
QString         Scene::getName()        {   return (name);          }
t_reg*          Scene::getRegionData()  {   return (region_data);   }

void    Scene::CloseThreads() {
    cout << "scene -> CloseThreads" << endl;

    emit changingRegion();
    cout << "signal emited" << endl;
}



void            Scene::alert_system(int sys_id) {
    cout << "ennemis in " << sys_id << endl;
    System  *sys = this->getSystem(sys_id);
    if (sys == NULL)
        return ;
    if (sys->getSysData()->isTracked == true)
        emit alert_audio();
    VisualAlert *visu = new VisualAlert(sys);
    connect(this, SIGNAL(changingRegion()), visu, SLOT(close_thread()));
    if (visu->isRunning() == false)
        visu->start(visu->LowPriority);
}

System*         Scene::getSystem(int id) {
    for (list<System*>::iterator it = system_list.begin(); it != system_list.end(); it++) {
        if ((*it)->getID() == id)
            return (*it);
    }
    cout << id << " : NULL" << endl;
    return (NULL);
}

void            Scene::setImage(QString image_path)
{
    view->setBackgroundBrush(QImage(QDir::currentPath() + image_path));
    view->setAlignment(Qt::AlignLeft|Qt::AlignTop);
    //view->setStyleSheet("border-image: url(" + QDir::currentPath() + image_path + ") 15 50 45 50 stretch stretch;");
    //scene->setSceneRect(200, 200, 800, 800);
}

void            Scene::drawSystems() {
    //cout << "Drawing.." << endl;
    if (region_data == NULL) {
        cout << "region_data NULL" << endl;
        scene->addRect(1,1,5,5,QPen(Qt::red, 1),QBrush());
        return ;
    }
    else
        scene->addRect(1,1,5,5,QPen(Qt::green, 1),QBrush());
    //cout << "Systems" << endl;
    for (list<t_sys>::iterator it = region_data->systems.begin(); it != region_data->systems.end(); it++) {
        System  *new_system = new System(&*it, this);
        system_list.push_back(new_system);
    }
    //cout << "Links\n";
    for (list<t_link>::iterator it = region_data->links.begin(); it != region_data->links.end(); it++) {
        //cout << (*it).sys1->name << " - " << (*it).sys2->name << endl;
        Link  *new_link = new Link(getSystem((*it).sys1->id), getSystem((*it).sys2->id), scene);
        link_list.push_back(new_link);
    }
    if (this->name.compare("Home") == 0) {
        QGraphicsTextItem   *softinfos = new QGraphicsTextItem();
        softinfos->setHtml(QString("<h2><u>EVE Online - Personnal Intel Map</u></h2>"
                           "<h4>This software was originally made for the corporation <u>Guardians of Toutatis</u> and is now public.<br>"
                           "If you encounter any bugs when using this software, please send an EVE mail to <b>Space Biquette</b> with subject \"Bug IntelMap\".</h4>"
                           "<h4><big>Configuration:</big><br>To make it work, you have to indicate the EVE chatlogs folder and the Intel channel name in the settings panel (bottom right), then select a region within the list.<br>"
                           "You can set audio alerts with your own audio file. There is a mute/unmute button but if you want to lower the volume, do it with the windows mixer (click play at least once to make it appear).<br>"
                           "These preferences are saved in the file data/preferences.</h4>"
                           "<h4><big>Features:</big><br>You can left-click on systems to tell the software to play an audio alert when a message is related to this system.<br>"
                           "When a system is registered for audio alerts, its rectangle will be filled with its security level color (as shown below).</h4>"
                           "<h4>Concerning the intel messages:<ul><li>Systems with space character may not be recognized. <u>If this occurs, EVE-mail that system to <b>Space Biquette</b></u></li>"
                           "<li>Partial system names are recognized, for example, writing \"dbrn\" will trigger dbrn-z. This works with a minimum of 4 letters.</li></ul>"));
        softinfos->setDefaultTextColor(QColor("#4888d8"));
        softinfos->setDefaultTextColor(QColor("#d9d9d9"));
        softinfos->setPos(QPointF(50,30));
        scene->addItem(softinfos);
    }
    //cout << "Drawing.. end\n";
}

