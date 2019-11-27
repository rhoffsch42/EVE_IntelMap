#include <data.h>

t_reg*		find_region(t_env *e, const char *name) {
    string  lowername(name);
    std::transform(lowername.begin(), lowername.end(), lowername.begin(), ::tolower);
    for (list<t_reg>::iterator it = e->regions.begin(); it != e->regions.end(); it++) {
        //cout << "reg compare: " << lowername << " & " << (*it).name << endl;
        if ((*it).name.compare(lowername) == 0) {
            //cout << "found region: " << lowername << endl;
            return (&*(it));
        }
    }
    return (NULL);
}

t_reg*		add_region(t_env *e, const char *name) {
    t_reg	new_region;

    new_region.name = strdup(name);
    std::transform(new_region.name.begin(), new_region.name.end(), new_region.name.begin(), ::tolower);
    e->regions.push_back(new_region);
    //cout << "add region: " << name << endl;
    return (&*(--(e->regions.end())));
}

t_sys*		add_system(t_reg *region, const char *name) {
    t_sys	new_system;

    new_system.name = strdup(name);
    std::transform(new_system.name.begin(), new_system.name.end(), new_system.name.begin(), ::tolower);
    new_system.security_level = 0.0f;
    new_system.id = 0;
    new_system.x = 0;
    new_system.y = 0;
    new_system.region_belonging = region;
    new_system.isTracked = false;
    region->systems.push_back(new_system);
  //  if (new_system.name.length() < 4)
    //    cout << " add_system: " << name << endl;
    return (&*(--(region->systems.end())));

}

bool    file_exists(string name, string flag) {
    if (FILE *file = fopen(name.c_str(), flag.c_str())) {
        fclose(file);
        return true;
    }
    return (false);
}

vector<string>  get_file_content(const char *filename) {
    char buf[500];
    char *val = _getcwd(buf, sizeof(buf));
    strcat(buf, filename);
    if (val)
        cout << buf << endl;
    ifstream	ifs(buf, ifstream::binary);

    if (!ifs)
        errexit(strcat(buf, strerror(errno)), IFS_FAIL);
    ifs.seekg(0, ifs.end);
    int	length = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    if (length == -1)
        errexit("Error, could'nt get file length.", FILE_READ);
    cout << length;
    char	*buffer = new char[length + 1];
    ifs.read(buffer, length);
    if (ifs)
        std::cout << ": All characters read successfully." << endl;
    else {
        cout << buffer << endl;
        std::cout << ": error: only " << ifs.gcount() << " could be read" << endl;
        errexit(NULL, FILE_READ);
    }
    ifs.close();
    buffer[length] = 0;

    vector<string>	tab;
    char	*tmp = strtok(buffer, "\n");
    int i = 1;
    while (tmp != NULL) {

        //cout << i << "_" << tmp << endl;
        tab.push_back(tmp);
        tmp = strtok(NULL, "\n");
        i++;
    }
    delete[] buffer;
    return (tab);
}

void	get_systems_data(t_env *e, const char *filename) {
    vector<string>	tab1 = get_file_content(filename);
    cout << "fuck";

    for (vector<string>::iterator it = tab1.begin() ; it != tab1.end(); ++it) {
        char * cstr = new char [it->length() + 1];
        strcpy(cstr, it->c_str());

        char *token = strtok(cstr, "/");
        int i = 0;
        t_reg*	region = NULL;
        t_sys*	eve_system = NULL;
        while (token != NULL) {
            i++;
            if (i == 1)
                if ((region = find_region(e, token)) == NULL)
                    region = add_region(e, token);
            if (i == 2)
                eve_system = add_system(region, token);
            if (i == 3)
                eve_system->security_level = atof(token);
            if (i == 4)
                eve_system->id = atoi(token);
            token = strtok(NULL, "/");
        }
       // if (eve_system->security_level < 10 && eve_system->name.length() < 4)
         //   cout << ":: " << eve_system->name << " sl: " << eve_system->security_level << endl;
        if (i != 4)
            errexit("Data Corrupted", 0);
        delete[] cstr;
    }
}

t_sys   *get_system_via_id(t_reg *region, int id) {
    for (list<t_sys>::iterator it = region->systems.begin(); it != region->systems.end(); it++) {
        if (id == (*it).id)
            return (&*it);
    }
    return (NULL);
}

void    get_systems_position(t_env *e) {
    for (vector<string>::iterator it = e->regionlist.begin() ; it != e->regionlist.end(); it++) {

        //cout << REGION_DATA + *it << endl;
        string    path(REGION_DATA + *it);
        vector<string>	tab = get_file_content(path.c_str());
        t_reg*	region = find_region(e, (*it).c_str());

        if (region != NULL) {
            //cout << region->name << endl;
            for (vector<string>::iterator it2 = tab.begin() ; it2 != tab.end(); it2++) {
                int     newsys = 0;
                int     oldx = 0;
                int     oldy = 0;
                //cout << *it2 << ": ";
                char * cstr = new char [it2->length() + 1];
                strcpy(cstr, it2->c_str());

                char *token = strtok(cstr, "/");
                int i = 0;
                t_sys*	eve_system = NULL;
                while (token != NULL) {
                    i++;
                    //cout << token << ".";
                    if (i == 1) {
                        eve_system = get_system_via_id(region, atoi(token));
                    if (eve_system == NULL) {
                        newsys = 1;
                        for (list<t_reg>::iterator it3 = e->regions.begin(); it3 != e->regions.end(); it3++) {
                            eve_system = get_system_via_id(&*it3, atoi(token));
                            if (eve_system != NULL)
                                break ;
                        }
                        if (eve_system == NULL) {
                            cout << "data corrupt: invalid system ID: " << token << " int: " << atoi(token) << endl;
                            break ;
                        }
                        oldx = eve_system->x;
                        oldy = eve_system->y;
                    }
                    else {;}//cout << eve_system->name << endl;
                    }
//debug                    if (eve_system->name.compare("xf-tql") == 0)
//debug                        cout << "&&&& " << eve_system->id << " " << token << endl;
                    if (i == 2)
                        eve_system->x = atoi(token);
                    if (i == 3)
                        eve_system->y = atoi(token);
                    token = strtok(NULL, "/");
                }
//debug                if (eve_system->name.compare("xf-tql") == 0)
//debug                    cout << "&&&& " << region->name << "~ " <<  eve_system->id << " " << eve_system->x << " " << eve_system->y << endl;
                if (newsys == 1) {
                    region->systems.push_back(*eve_system);
                    eve_system->x = oldx;
                    eve_system->y = oldy;
                }
                //cout << endl;
            }//for
        }//region != NULL
    }
}

void    get_systems_links(t_env *e) {
    for (vector<string>::iterator it = e->regionlist.begin() ; it != e->regionlist.end(); it++) {

        //cout << REGION_LINKS + *it << endl;
        string    path(REGION_LINKS + *it);
        vector<string>	tab = get_file_content(path.c_str());
        t_reg*	region = find_region(e, (*it).c_str());

        if (region != NULL) {
            //cout << region->name << endl;
            for (vector<string>::iterator it2 = tab.begin() ; it2 != tab.end(); it2++) {
                //cout << *it2 << endl;
                char * cstr = new char [it2->length() + 1];
                strcpy(cstr, it2->c_str());

                char *token = strtok(cstr, "/");
                int i = 0;
                t_sys*	eve_system;
                t_link  link;
                while (token != NULL) {
                    i++;
                    //cout << token << ".";
                    eve_system = get_system_via_id(region, atoi(token));
                    if (eve_system == NULL) {
                        cout << "data corrupt: invalid system id: " << token << " int: " << atoi(token) << endl;
                        break ;
                    }
                    if (i == 1)
                        link.sys1 = eve_system;//cout << eve_system->name << endl;
                    else if (i == 2)
                        link.sys2 = eve_system;//cout << eve_system->name << endl;
                    token = strtok(NULL, "/");
                }
                region->links.push_back(link);
            }//for
        }//region != NULL
    }
}
