#ifndef TRACKER_H
#define TRACKER_H

#include "intel.h"
#include "data.h"
#include <dirent.h>
#include <time.h>
#include <QApplication>
#include <QThread>
#include <ctime>
#include <fstream>
#include <iostream>
#include <time.h>
#include <string>
#include <vector>

class IntelMap;

class Tracker : public QThread {
    Q_OBJECT

public:
    Tracker(t_env *e, IntelMap *fenetre_ptr);
    ~Tracker();

    string		replace_chars(string msg);
    void		check_intel(t_reg *regiondata, t_msg message);

    int         get_timestamp(string timestr);
    int         valid(string line);
    void        read_log(t_env *e, string filename);
    string      *get_log_file(QString dirpath, QString chan);

    string		timeago(int	timestamp);
    int         start_track(t_env *e, IntelMap *fenetre);

signals:
    void    update();
    void    alert(int sys_name);

protected:
    void    run() Q_DECL_OVERRIDE;

private:
    IntelMap    *fenetre;
    t_env       *env;

};

#endif // TRACKER_H
