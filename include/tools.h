#ifndef _TOOLS
#define _TOOLS

#include <string>
#include <map>
#include "stem.h"

using namespace std;
extern class tools *Global_Tools;

class tools {
public:
    tools(string stop_file);
    int is_stop(string word, int ignore_case = 1);
    string stem(string word);
private:
    map<string, int> stop_list;
    stemmer stem_proc;
    void load_stop(const char *f);
};

#endif
