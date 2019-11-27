#include "intel.h"
#include <iostream>

IntelMap::IntelMap(Scene *scene, t_env *e) : QWidget() {
    data = e;
    current_scene = scene;
    scene_list.push_back(scene);
    this->setWindowTitle("EVE Online - Personnal Intel Map - Beta 20.09.2016");
    this->setWindowIcon(QIcon(QDir::currentPath() + MAIN_ICON));
    this->showMaximized();

    QPalette pal = this->palette();
    pal.setColor(QPalette::Window, MAIN_BG_COLOR);
    this->setPalette(pal);

    //this->setMinimumSize(500, 500);

    // Constructor: directory button
    dir_button = new QPushButton("Choose logs directory", this);
    dir_button->setFont(QFont("Verdana", 8));
    dir_button->setToolTip("Click here to indicate where are the EVE logs file");
    dir_button->setCursor(Qt::PointingHandCursor);
    dir_button->setIcon(QIcon(QDir::currentPath() + DIR_ICON));
    //dir_button->setIconSize(QSize(64, 64));
     QWidget::connect(dir_button, SIGNAL(clicked()), this, SLOT(choose_dir()));

    // Constructor: channel button
    chan_button = new QPushButton("Choose channel", this);
    chan_button->setFont(QFont("Verdana", 8));
    chan_button->setToolTip("Click here to choose whitch channel should be tracked");
    chan_button->setCursor(Qt::PointingHandCursor);
    chan_button->setIcon(QIcon(QDir::currentPath() + CHAN_ICON));
    connect(chan_button, SIGNAL(clicked()), this, SLOT(choose_intel()));

    // Constructor: choose sound button
    sound_button = new QPushButton("Choose audio", this);
    sound_button->setFont(QFont("Verdana", 8));
    sound_button->setToolTip("Click here to choose a personnal audio file for your alerts");
    sound_button->setCursor(Qt::PointingHandCursor);
    sound_button->setIcon(QIcon(QDir::currentPath() + VOLUME_ICON));
    connect(sound_button, SIGNAL(clicked()), this, SLOT(choose_sound()));

    // Constructor: Play sound button
    play_button = new QPushButton("Play", this);
    play_button->setFont(QFont("Verdana", 8));
    play_button->setToolTip("Click here to play the alert");
    play_button->setCursor(Qt::PointingHandCursor);
    play_button->setMaximumWidth(40);
    connect(play_button, SIGNAL(clicked()), this, SLOT(play_sound()));

    // Constructor: Player
    player = new QMediaPlayer(this);
    player->setMedia(QUrl::fromLocalFile(QDir::currentPath() + DEFAULT_MP3));
    //QMessageBox::information(this, "audio path", QDir::currentPath() + "/alert.mp3");
    player->setVolume(50);

    // Constructor: Mute alerts button
    mute_button = new QPushButton("", this);
    mute_button->setFont(QFont("Verdana", 8));
    mute_button->setToolTip("Click here to mute/unmute the alert");
    mute_button->setCursor(Qt::PointingHandCursor);
    mute_button->setIcon(QIcon(QDir::currentPath() + VOLUME_ICON));
    connect(mute_button, SIGNAL(clicked()), this, SLOT(mute_sound()));

    // Constructor: Regions list
    region_list = new QComboBox(this);
    region_list->setMaximumWidth(150);
    //region_list->addItem(scene->getName());
    connect(region_list, SIGNAL(currentIndexChanged(QString)), this, SLOT(change_region(QString)));

    // Constructor: chat history
    chat_history = new QTextEdit(this);
    chat_history->setReadOnly(true);
    chat_history->setStyleSheet(BG_CHAT);
    chat_history->setMaximumWidth(LAYOUT_MAX_W);

    QGraphicsDropShadowEffect *effect = new QGraphicsDropShadowEffect();
    effect->setBlurRadius(BLUR_RADIUS);
    effect->setOffset(0, 0);
    effect->setColor(BLUR_COLOR);
    chat_history->setGraphicsEffect(effect);

    // Constructor: history clear button
    clear_button = new QPushButton("Clear");
    clear_button->setFont(QFont("Verdana", 8));
    clear_button->setToolTip("Click here to clear the chat history");
    clear_button->setCursor(Qt::PointingHandCursor);
    clear_button->setMaximumWidth(50);
    QGridLayout *container = new QGridLayout(chat_history);
    container->addWidget(clear_button, 0, 0, Qt::AlignTop | Qt::AlignRight);
    container->setContentsMargins(0,5,20,0);
    //clear_button->setIcon(QIcon("images/logs_icon.png"));
    connect(clear_button, SIGNAL(clicked()), this, SLOT(clear_history()));


    layout = new QHBoxLayout(this);
    QVBoxLayout *layout_info = new QVBoxLayout;
    layout->addWidget(current_scene->getView());
    layout->addLayout(layout_info);

    layout_info->addWidget(chat_history);
    QWidget     *setting_container = new QWidget;
    setting_container->setObjectName("setting_container");
    this->setStyleSheet(BG_SETTINGS);
    setting_container->setMaximumSize(LAYOUT_MAX_W, 150);
    layout_info->addWidget(setting_container);

    QGraphicsDropShadowEffect *effectSettings = new QGraphicsDropShadowEffect();
    effectSettings->setBlurRadius(BLUR_RADIUS);
    effectSettings->setOffset(0, 0);
    effectSettings->setColor(BLUR_COLOR);
    setting_container->setGraphicsEffect(effectSettings);


    chan_description = new QLineEdit("");
    dir_description = new QLineEdit ("");
    sound_description = new QLineEdit(QDir::currentPath() + DEFAULT_MP3);
    chan_description->setReadOnly(true);
    dir_description->setReadOnly(true);
    sound_description->setReadOnly(true);


    if (file_exists(string(qPrintable(QDir::currentPath())) + PREFS_DATA, "r")) {
        vector<string>  pref = get_file_content(PREFS_DATA);
        cout << PREFS_DATA << " " << pref.size() << " lines" << endl;
        for (vector<string>::iterator it = pref.begin(); it != pref.end(); it++) {
            cout << *it << endl;
            (*it)[(*it).length() - 1] = 0;
            if (strncmp(PREFS_CHAN, (*it).c_str(), strlen(PREFS_CHAN)) == 0)
                chan_description->setText((*it).c_str() + strlen(PREFS_CHAN));
            if (strncmp(PREFS_DIR, (*it).c_str(), strlen(PREFS_DIR)) == 0)
                dir_description->setText((*it).c_str() + strlen(PREFS_DIR));
            if (strncmp(PREFS_AUDIO, (*it).c_str(), strlen(PREFS_AUDIO)) == 0) {
                sound_description->setText((*it).c_str() + strlen(PREFS_AUDIO));
                player->setMedia(QUrl::fromLocalFile(sound_description->text()));
            }
        }
        cout << PREFS_DATA << " read and accepted" << endl;
    } else {
        cout << PREFS_DATA << " not reachable" << endl;
    }
    if (dir_description->text().length() == 0) {
        char* docdir = getenv("USERPROFILE");
        if(docdir)
        {
            dir_description->setText(QString(QString(docdir) + USER_EVE_LOGS).replace("\\", "/", Qt::CaseSensitive));
        }
    }

    QPalette palette;
    palette.setColor(QPalette::Text, Qt::red);
    DIR*    rep = NULL;
    if ((rep = opendir(qPrintable(dir_description->text()))) == NULL) {
        dir_description->setPalette(palette);
    } else { closedir(rep); }
    if (file_exists(string(qPrintable(sound_description->text())), "r") ==  false)
        sound_description->setPalette(palette);

    QVBoxLayout *layout_settings = new QVBoxLayout(setting_container);
    QHBoxLayout *layout_bot1 = new QHBoxLayout;
    QHBoxLayout *layout_bot2 = new QHBoxLayout;
    QHBoxLayout *layout_bot3 = new QHBoxLayout;
    QHBoxLayout *layout_bot4 = new QHBoxLayout;
    layout_settings->addLayout(layout_bot1);
    layout_bot1->addWidget(region_list);
    QWidget     *space = new QWidget();
    space->setStyleSheet(EVE_LOGO);
    layout_bot1->addWidget(space);
    layout_settings->addLayout(layout_bot2);
    layout_bot2->addWidget(chan_button);
    layout_bot2->addWidget(chan_description);
    layout_settings->addLayout(layout_bot3);
    layout_bot3->addWidget(dir_button);
    layout_bot3->addWidget(dir_description);
    layout_settings->addLayout(layout_bot4);
    layout_bot4->addWidget(sound_button);
    layout_bot4->addWidget(play_button);
    layout_bot4->addWidget(mute_button);
    layout_bot4->addWidget(sound_description);
    this->setLayout(layout);

    //QMessageBox::information(this, "Important", "You should read, really..");
    if (DEBUG_METHODS == 1)
        cout << "----------DEBUG intelMap created" << endl;
}

string  timeago2(int timestamp) {
    time_t	now;
    time(&now);
    int	diff = ((int)now - (timestamp - EVETIME_DIFF * 3600)) / 60;
    //cout << "diff: " << diff << endl;
    if (diff == 0)
        return ("now\t");
    char    tmp[20];
    tmp[19] = 0;
    int i = 18;
    while (diff != 0) {
        tmp[i] = diff % 10 + 48;
        diff = diff / 10;
        i--;
        if (i < 0)
            break ;
    }
    return (string(string(tmp + i + 1) + " min\t"));
}

void    IntelMap::updateChat() {
    int val = chat_history->verticalScrollBar()->value();
    if (val == chat_history->verticalScrollBar()->maximum())
        val = -1;
    chat_history->clear();
    data->mutex.lock();
    for (list<t_msg>::iterator it = data->big_history.begin(); it != data->big_history.end(); it++) {
        chat_history->setTextColor("red");
        if (atoi(timeago2(it->timestamp).data()) > 5)
            chat_history->setTextColor("brown");
        if (atoi(timeago2(it->timestamp).data()) > 15)
            chat_history->setTextColor("black");
        chat_history->append(timeago2(it->timestamp).data());
        chat_history->setTextColor("black");
        chat_history->insertPlainText((it->msg.data()));
    }
    if (val == -1)
        chat_history->verticalScrollBar()->setValue(chat_history->verticalScrollBar()->maximum());
    else
        chat_history->verticalScrollBar()->setValue(val);
    data->mutex.unlock();
}

void    IntelMap::choose_dir() {
    QString dir = QFileDialog::getExistingDirectory(this);
    if (dir.isEmpty())
        QMessageBox::critical(this, "Directory: error", "Nothing selected");
    else {
        data->mutex.lock();
        dir_description->setText(dir);
        QPalette palette;
        palette.setColor(QPalette::Text, Qt::red);
        DIR*    rep = NULL;
        if ((rep = opendir(qPrintable(dir_description->text()))) == NULL) {
            dir_description->setPalette(palette);
        } else {
            palette.setColor(QPalette::Text, Qt::black);
            dir_description->setPalette(palette);
            closedir(rep);
        }
        data->mutex.unlock();
        save_preferences();
        //QMessageBox::information(this, "Directory", "You selected :\n" + dir_description->text());
    }
}

void    IntelMap::choose_intel() {
    QString chan = QInputDialog::getText(this, "Intel channel", "Type the name of the IG intel channel you want to be tracked:");
    if (chan.isEmpty())
        QMessageBox::critical(this, "Intel channel: error", "Nothing selected");
    else {
        data->mutex.lock();
        chan_description->setText(chan);
        data->mutex.unlock();
        save_preferences();
        //QMessageBox::information(this, "Intel chan", "You selected :\n" + chan_description->text());
    }
}

void    IntelMap::change_region(QString region_name) {
    if (region_name.compare(current_scene->getName()) == 0)
        return ;
    //QMessageBox::information(this, "Intel chan", "Region index selected :\n" + QString::number(index));
    Scene   *new_scene = new Scene(region_name, getRegionData(region_name));
    new_scene->connect(this->track, SIGNAL(alert(int)), new_scene, SLOT(alert_system(int)));
    new_scene->connect(new_scene, SIGNAL(alert_audio()), this, SLOT(play_sound()));
    this->changeScene(new_scene);
}

void    IntelMap::changeScene(Scene *scene) {
    layout->removeWidget(current_scene->getView());
    current_scene->CloseThreads();
    usleep(100000);
    delete current_scene;
    current_scene = scene;
    //scene->getView()->setMaximumSize(QSize(1500, 800));
    layout->insertWidget(0, current_scene->getView());
}

void    IntelMap::choose_sound() {
    QString sound = QFileDialog::getOpenFileName(this, "Choose an audio file for alerts (mp3)", QString(), "Audio (*.mp3)");
    if (sound.isEmpty())
        QMessageBox::critical(this, "Audio file: error", "Nothing selected");
    else {
        data->mutex.lock();
        sound_description->setText(sound);
        data->mutex.unlock();
        player->setMedia(QUrl::fromLocalFile(sound_description->text()));
        save_preferences();
        //QMessageBox::information(this, "Audio file", "You selected :\n" + sound_description->text());
    }

}

void    IntelMap::save_preferences() {
    FILE *pref_file;
    pref_file = fopen(string(qPrintable(QDir::currentPath()) + string(PREFS_DATA)).c_str(),"w");
    if (pref_file != NULL) {
        cout << "Updating " << PREFS_DATA << endl;
        //data->mutex.lock();
        string  data(string("audio:") + qPrintable(sound_description->text()) + string("\n") + \
                     string("chan:") + qPrintable(chan_description->text()) + string("\n") + \
                     string("dir:") + qPrintable(dir_description->text()) + string("\n"));
        fwrite(data.c_str(), 1, data.length(), pref_file);
       // data->mutex.unlock();
        fclose (pref_file);
        cout << "Updated! " << PREFS_DATA << endl;
    }
}

void    IntelMap::play_sound() {
    player->stop();
    player->play();
    //QMessageBox::information(this, "Play sound", "Alert is playing...");
}

void    IntelMap::mute_sound() {
    if (player->isMuted() == true) {
        player->setMuted(false);
        mute_button->setIcon(QIcon(QDir::currentPath() + VOLUME_ICON));
        //QMessageBox::information(this, "mute", "not muted");
    } else {
        player->setMuted(true);
        mute_button->setIcon(QIcon(QDir::currentPath() + VOLUME_MUTE_ICON));
        //QMessageBox::information(this, "mute", "muted");
    }
}

void    IntelMap::clear_history() {
    data->mutex.lock();
    data->big_history.clear();
    data->mutex.unlock();
    //QMessageBox::information(this, "Chat History", "History cleared");
}

void    IntelMap::add_region(QString name)          {    region_list->addItem(name);        }
void    IntelMap::add_scene(Scene *new_scene)       {    scene_list.push_back(new_scene);   }
void    IntelMap::setTracker(Tracker *track_thread) {    this->track = track_thread;        }

QHBoxLayout*    IntelMap::getLayout() {         return (layout);            }
Scene*          IntelMap::getCurrentScene() {   return (current_scene);     }
Scene*          IntelMap::getScene(int index) { return (scene_list[index]); }
QString         IntelMap::getDir() {    return (dir_description->text());   }
QString         IntelMap::getChan() {  return (chan_description->text());   }
QTextEdit*      IntelMap::getChatHistory() {    return (chat_history);      }
Tracker*        IntelMap::getTracker() {        return (track);             }

t_reg*          IntelMap::getRegionData(QString region_name) {
    string  lowername(qPrintable(region_name));
    std::transform(lowername.begin(), lowername.end(), lowername.begin(), ::tolower);
    cout << "Looking data for " << qPrintable(region_name) << ".. ";
    for (list<t_reg>::iterator it = data->regions.begin(); it != data->regions.end(); it++) {
        if (lowername.compare((*it).name.c_str()) == 0) {
            cout << "Found\n";
            return (&*it);
        }
    }
    cout << "Notfound\n";
    return (NULL);
}
