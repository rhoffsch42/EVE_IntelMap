#include "tracker.h"

Tracker::Tracker(t_env *e, IntelMap *fenetre_ptr) {
    env = e;
    fenetre = fenetre_ptr;
    if (DEBUG_METHODS == 1)
        cout << "----------DEBUG tracker created" << endl;
}


Tracker::~Tracker() {

}

void    Tracker::run() {
    start_track(env, fenetre);

}

string		Tracker::replace_chars(string msg) {
    string	tmp(msg);
    int		j = 0;

    for (size_t i = 0; i < msg.size(); i++)
    {
        if (strchr(REPLACE_CHAR, tmp[i]) != NULL)
        {
            if (j > 0)
                tmp[i] = ' ';
            if (tmp[i] == '>')
                j++;
        }
    }
    std::transform(tmp.begin(), tmp.end(), tmp.begin(), ::tolower);
    return (tmp);
}

void		Tracker::check_intel(t_reg *regiondata, t_msg message) {
    if (regiondata == NULL) {
        cout << "regiondata null" << endl;
        return ;
    }
    cout << "Scanning: " << regiondata->name << endl;
    string		replace = replace_chars(message.msg);
    char        *explode = strtok((char *)replace.c_str(), ">");
    explode = strtok(NULL, ">");

    //cout << "Scanning: " << explode << endl;
    explode = strtok(explode, " ");
    while (explode != NULL) {
        for (list<t_sys>::iterator it2 = regiondata->systems.begin(); it2 != regiondata->systems.end(); it2++) {
            /*  cout << "[ " << it2->name << " & " << explode << " , " << strlen(it2->name.c_str()) << " | " << strlen(explode) << endl;
             *                    for (int i = 0; i != strlen(explode); i++) {
             *                        cout << (int)(explode[i]) << ".";
             *                    }
             *                    cout << endl;
             */
            if (strcmp(explode, it2->name.c_str()) == 0 || (strlen(explode) >= 4 && strncmp(explode, it2->name.c_str(), strlen(explode)) == 0)) {
                emit alert(it2->id);
                QThread::usleep(100000);
                cout << "[ALERT] " << regiondata->name << " " << it2->name << ": " << message.msg << endl;
                it2->history.push_back(message.msg);
            }
        }
        explode = strtok(NULL, " ");
    }
    //cout << "End Scanning" << endl;
}

time_t timegm(struct tm * a_tm)
{
    time_t ltime = mktime(a_tm);
    struct tm tm_val;
    gmtime_s(&tm_val, &ltime);
    int offset = (tm_val.tm_hour - a_tm->tm_hour);
    if (offset > 12)
    {
    offset = 24 - offset;
    }
    time_t utc = mktime(a_tm) - offset * 3600;
    return utc;
}

int		Tracker::get_timestamp(string timestr) {
    time_t		timer;
    time(&timer);
    struct tm	*temps = gmtime(&timer);

    temps->tm_year = atoi(timestr.substr(0, 4).c_str()) - 1900;
    temps->tm_mon = atoi(timestr.substr(5, 2).c_str()) - 1;
    temps->tm_mday = atoi(timestr.substr(8, 2).c_str());
    temps->tm_hour = atoi(timestr.substr(11, 2).c_str());
    temps->tm_min = atoi(timestr.substr(14, 2).c_str());
    temps->tm_sec = atoi(timestr.substr(17, 2).c_str());

    // cout << timestr << endl;
    // cout << "year:\t" << temps->tm_year << endl;
    // cout << "month:\t" << temps->tm_mon << endl;
    // cout << "day:\t" << temps->tm_mday << endl;
    // cout << "hour:\t" << temps->tm_hour << endl;
    // cout << "min:\t" << temps->tm_min << endl;
    // cout << "sec:\t" << temps->tm_sec << endl;
    // cout << "mktime: " << mktime(temps) << endl;
    // cout << "current: " << ctime(&timer) << endl;
   // cout << "||| " << mktime(temps) - timer << endl;
   // cout << "|| " << timegm(temps) - timer << endl;
    return (timegm(temps));
}

int		Tracker::valid(string line) {
    if (line.size() > MSG_BEGIN)
    {
        if (line[0] == '[' && line[MSG_BEGIN - 2] == ']')
        {
            return (1);
            // if (history.size() == 0)
                // return (1);
            // int diff_timestamp = get_timestamp(line.substr(TIME_BEGIN, TIME_END)) - history.back().timestamp;
            // if (diff_timestamp > 0)
                // return (1);
            // if (diff_timestamp < 0)
                // return (0);
            // if (diff_timestamp == 0)
            // {
                // if (line.substr(MSG_BEGIN).compare(history.back().msg) != 0)
                    // return (1);
            // }
        }
    }
    return (0);
}

void	Tracker::read_log(t_env *e, string filename) {
    cout << "Trying to open: " << filename << endl;
    static string   old = "";
    ifstream		myfile(filename.c_str());
    list<string>	conteneur;
    string			line;
    static int		bytes_read = 0;

    if (old.compare(filename) != 0) {
        bytes_read = 0;
        cout << "Not same file" << endl;
    } else {
        cout << "Same file" << endl;
    }
    old = filename;

    if (myfile.is_open())
    {
        myfile.seekg (0, myfile.end);
        int length = myfile.tellg();

        myfile.seekg(bytes_read, myfile.beg);
        if (length < bytes_read)
        {
            myfile.seekg (0, myfile.beg);
            bytes_read = 0;
        }
        while (getline(myfile, line))
        {
            unsigned int j = 0;
            //cout << "\\_" << line << "_/" << endl;
            for (unsigned int i = 0; i != line.size(); i++) {
                if (line[i] > 0 && line[i] != 13) {
                    line[j] = line[i];
                    j++;
                }
            }
            while (j != line.size())
                line[j++] = 0;
            if (valid(line) == 1)
                conteneur.push_back(line);
            bytes_read += line.length();
        }
        myfile.close();
    }
    else {
        cout << "Could not open: " << filename << endl;
        return ;
    }

    //cout << "Add to historys... ";
    for (list<string>::iterator it = conteneur.begin(); it != conteneur.end(); it++)
    {
        //cout << "start\t";
        t_msg		message;

        message.msg = it->substr(MSG_BEGIN);
        message.timestamp = get_timestamp(it->substr(TIME_BEGIN, TIME_END));
        e->history.push_back(message);
        e->big_history.push_back(message);
        //cout << "add:\t" << message.timestamp << "\t\\" << message.msg << "/" << endl;
    }
}

string		Tracker::timeago(int timestamp) {
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
    return (string(string(tmp + i + 1) + "min\t"));
}

string*      Tracker::get_log_file(QString dirpath, QString chan) {
    DIR*            rep = NULL;
    struct dirent*	file = NULL;

    if (dirpath.size() == 0 || chan.size() == 0) {
        cout << "Empty directory or chan name" << endl;
        return (NULL);
    }
    if ((rep = opendir(qPrintable(dirpath))) == NULL) {
        cout << "Failed to open directory: \\" << qPrintable(dirpath) << "/" << endl;
        return (NULL);
    }
    string	*filename = NULL;
    while ((file = readdir(rep)) != NULL) {
        if (strncmp(file->d_name, qPrintable(chan), chan.size()) == 0) {
            if (filename)
                delete filename;
            filename = new string(file->d_name);
        }
    }
    closedir(rep);
    if (filename == NULL) {
        cout << "No logfile found" << endl;
        return (NULL);
    }
    string *complete = new string(qPrintable(dirpath) + string("/") + *filename);
    delete filename;
    return (complete);
}

int         Tracker::start_track(t_env *e, IntelMap *fenetre) {
    if (DEBUG_METHODS == 1)
        cout << "----------DEBUG tracker launched" << endl;
    while (1)
    {
       cout << endl;
       // SECURE // comment this to avoid date check
       e->mutex.lock();
       string   *filename = get_log_file(fenetre->getDir(), fenetre->getChan());
       if (filename != NULL) {
           read_log(e, *filename);
           t_reg    *regiondata = fenetre->getCurrentScene()->getRegionData();
           if (regiondata != NULL)
               cout << "###   " << regiondata->name << endl;
           else
               cout << "###   nope" << endl;
           if (e->history.size() < 5)
               for (list<t_msg>::iterator it = e->history.begin(); it != e->history.end(); it++)
                   check_intel(fenetre->getCurrentScene()->getRegionData(), *it);

        /*   for (list<t_msg>::iterator it = e->big_history.begin(); it != e->big_history.end(); it++)
               cout << it->timestamp << "\t" << timeago(it->timestamp) << "\t" << it->msg << endl;
*/

           for (list<t_reg>::iterator it = e->regions.begin(); it != e->regions.end(); it++) {
               for (list<t_sys>::iterator it2 = it->systems.begin(); it2 != it->systems.end(); it2++) {
                   if (it2->history.size() > 0) {
                       cout << it2->name << endl;
                       for (list<string>::iterator it3 = it2->history.begin(); it3 != it2->history.end(); it3++)
                           cout << "\t- " << *it3 << endl;
                   }
               }
           }
           e->history.clear();
           //cout << "----------------------------------------" << endl;
           delete filename;
       }
       e->mutex.unlock();
       emit update();
       QThread::sleep(REFRESH_TIME);

    }

    string		time1 = "[ 2016.08.08 19:53:38 ] Intel Kamms > Hello there.";
    string		time2 = "[ 2016.08.08 19:53:38 ] Intel Kamms > ddsds";
    return (OK);
}
