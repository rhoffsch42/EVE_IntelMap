
#include <QApplication>
#include "intel.h"
#include "scene.h"
#include "system.h"
#include "data.h"
#include "tracker.h"

void	errexit(const char *msg, int errorcode)
{
    if (msg)
        cerr << msg << endl;
    exit(errorcode);
}

void	print_data(list<t_reg> regions)
{
    for (list<t_reg>::iterator it = regions.begin(); it != regions.end(); it++)
    {
        cout << "- " << it->name << " #" << it->systems.size() << endl;
        for (list<t_sys>::iterator it2 = it->systems.begin(); it2 != it->systems.end(); it2++) {
            cout << "\t" << it2->name << " (" << it2->security_level << ") id: " << it2->id << endl;
        }
    }
}

string		timeago(int	timestamp)
{
    time_t	now;
    time(&now);
    int	diff = ((int)now - (timestamp - EVETIME_DIFF * 3600)) / 60;
    if (diff == 0)
        return ("now");
    char    buf1[20];
    itoa(diff, buf1, 10);
    strcat(buf1, "min");
    string buf2(buf1);
    return (buf2);
}

int main_window(int argc, char *argv[], t_env *e)
{
    QApplication app(argc, argv);

    t_reg   *ptr = &*(e->regions.begin());
    for (list<t_sys>::iterator it = ptr->systems.begin(); it != ptr->systems.end(); it++) {
        if ((*it).id > 5)
            (*it).isTracked = 1;
    }
    Scene       *scene = new Scene("Home", ptr);
    IntelMap    *window = new IntelMap(scene, e);
    window->show();

    for(unsigned int i = 0; i != e->regionlist.size(); i++) {
        //cout << e->regionlist[i] << endl;
        window->add_region(e->regionlist[i].c_str());
    }
    Tracker *track = new Tracker(e, window);
    window->setTracker(track);
    window->connect(track, SIGNAL(update()), window, SLOT(updateChat()));
    scene->connect(track, SIGNAL(alert(int)), scene, SLOT(alert_system(int)));
    scene->connect(scene, SIGNAL(alert_audio()), window, SLOT(play_sound()));
    if (track->isRunning() == false)
        track->start(track->LowPriority);
    return app.exec();
}

int main(int argc, char *argv[])
{
    t_env	*e = new t_env[1];

    get_systems_data(e, COMPLETE_DATA);
    //print_data(e->regions);
    e->regionlist = get_file_content(REGIONLIST);
    get_systems_position(e);
    //print_data(e->regions);
    get_systems_links(e);
 /*   cout << "Links:" << endl;
    for (list<t_reg>::iterator it_reg = e->regions.begin(); it_reg != e->regions.end(); it_reg++) {
        cout << (*it_reg).name << ":     \t";
        if ((*it_reg).links.size() == 0)
            cout << "No ";
        cout << "links" << endl;
        for (list<t_link>::iterator it_link = (*it_reg).links.begin(); it_link != (*it_reg).links.end(); it_link++) {
            //cout << (*it_link).sys1->name << " - " << (*it_link).sys2->name << endl;
        }
    }*/
    //return (0);
    cout << "QT side begin" << endl;
    /* comment this to avoid date check
        SECURE
        cout << "timestamp: " << now << endl;
        cout << (now - SEPT_9_0000) / 3600 << endl;
    */
    return (main_window(argc, argv, e));
}
