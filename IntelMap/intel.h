#ifndef INTEL_H
#define INTEL_H

#include <data.h>
#include <iostream>
#include <vector>
#include <QApplication>
#include <QWidget>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
#include <QComboBox>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QtMultimedia/QMediaPlayer>
#include <QSound>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QTextTable>
#include <QThread>
#include <QScrollBar>
#include <dirent.h>
#include "scene.h"
#include "tracker.h"

class Tracker;

class IntelMap : public QWidget
{
    Q_OBJECT

public:
    IntelMap(Scene *scene, t_env *e);
    QHBoxLayout     *getLayout();
    Scene           *getCurrentScene();
    Scene           *getScene(int index);
    t_reg           *getRegionData(QString region_name);
    QString         getDir();
    QString         getChan();
    QTextEdit       *getChatHistory();
    Tracker         *getTracker();

    void            changeScene(Scene *scene);
    void            add_scene(Scene *new_scene);
    void            add_region(QString name);
    void            save_preferences();
    void            setTracker(Tracker *track_thread);

public slots:
    void            updateChat();
    void            choose_dir();
    void            choose_intel();
    void            choose_sound();
    void            change_region(QString region_name);
    void            play_sound();
    void            mute_sound();
    void            clear_history();

private:
    t_env                   *data;
    Tracker                 *track;
    // BOX (layoutV)
    QHBoxLayout             *layout;
    Scene                   *current_scene;
    std::vector<Scene*>     scene_list;

    // BOX botton right
    QComboBox       *region_list;
    QPushButton     *dir_button;
    QLineEdit       *dir_description;
    QPushButton     *chan_button;
    QLineEdit       *chan_description;
    QPushButton     *sound_button;
    QLineEdit       *sound_description;
    QPushButton     *play_button;
    QPushButton     *mute_button;
    QMediaPlayer    *player;

    // BOX bottom left
    QTextEdit       *chat_history;
    QPushButton     *clear_button;
};

#endif // INTEL_H
