#ifndef DATA_H
#define DATA_H


#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <list>
#include <stdlib.h>
#include <string>
#include <time.h>
#include <unistd.h>
#include <vector>
#include <algorithm>
#include <QMutex>

#define DEBUG_METHODS   1
#define WEEK_SEC        604800
#define SEPT_9_0000     1473370348
#define SECURE          time_t now;time(&now);if (now > SEPT_9_0000 + WEEK_SEC * 19){exit(0);}

#define OK              0
#define BAD_ARG 		1
#define IFS_FAIL		2
#define FILE_READ		3

#define MSG_BEGIN		24
#define TIME_BEGIN		2
#define TIME_END		19
#define EVETIME_DIFF	0
#define REFRESH_TIME	3
#define REPLACE_CHAR	"!@#$%^&*()+=_`~:;\"\'[]{}\\|,.<>?/"

#define MAIN_ICON           "/images/logo_minmatar.png"
#define DIR_ICON            "/images/dir_icon.png"
#define CHAN_ICON           "/images/chan_icon.png"
#define VOLUME_ICON         "/images/volume_icon.png"
#define VOLUME_MUTE_ICON    "/images/volume_mute_icon.png"
#define DEFAULT_MP3         "/alert.mp3"
#define BG_CHAT             "background-image: url(images/graybg.jpg)"
#define BG_SETTINGS         "QWidget#setting_container {background-image: url(images/bg1.png)}"
#define EVE_LOGO            "border-image: url(images/eve.png) 15 50 45 50 stretch stretch;"
#define BLUR_COLOR          Qt::black
#define MAIN_BG_COLOR       QColor("#292830")
#define BLUR_RADIUS         10

#define USER_EVE_LOGS       "/Documents/EVE/logs/Chatlogs/"
#define COMPLETE_DATA       "/data/sysdata"
#define REGIONLIST          "/data/regionlist"
#define BG_PATH             "/images/bg/"
#define REGION_DATA         "/data/regions/"
#define REGION_LINKS        "/data/links/"

#define PREFS_DATA      "/data/preferences"
#define PREFS_DIR       "dir:"
#define PREFS_CHAN      "chan:"
#define PREFS_AUDIO     "audio:"

#define MARGINX         50
#define MARGINY         50
#define RECT_LINE_PX    2
#define RECT_WIDTH      50
#define RECT_HEIGHT     20
#define POS_COEF_X      1.3
#define POS_COEF_Y      1.1
#define NULLSEC_COLOR   "#C23B22"
#define LOWSEC_COLOR    "#FFB347"
#define HIGHTSEC_COLOR  "#77DD77"
#define RECT_COLOR      "#000000"
#define LAYOUT_MAX_W    400

using namespace std;

typedef struct s_reg	t_reg;

typedef struct		s_msg
{
    string			msg;
    int				timestamp;
}					t_msg;

typedef struct		s_sys
{
    string			name;
    float			security_level;
    int				id;
    int             x;
    int             y;
    t_reg			*region_belonging;
    list<string>	history;
    bool            isTracked;
}					t_sys;

typedef struct		s_link
{
    t_sys           *sys1;
    t_sys           *sys2;
}                   t_link;

typedef struct		s_reg
{
    string			name;
    list<t_sys>		systems;
    list<t_link>    links;
}					t_reg;

typedef struct		s_env
{
    QMutex          mutex;
    vector<string>  regionlist;
    list<t_reg>		regions;
    list<t_msg>		history;
    list<t_msg>		big_history;
}					t_env;

void            errexit(const char *msg, int errorcode);
bool            file_exists(string name, string flag);
vector<string>  get_file_content(const char *filename);
void            get_regionlist(const char *filename);
void            get_systems_data(t_env *e, const char *filename);
void            get_systems_links(t_env *e);
void            get_systems_position(t_env *e);
void            read_log(t_env *e, const char *filename);
void            check_intel(t_env *e, t_msg message);

int			init_qt(int argc, char **argv);

#endif // DATA_H
