#ifndef TOOLS
#define TOOLS

#include <string>
#include <map>
#include "stem.h"

using namespace std;

class tools {
public:
    tools(string stop_file);
    int is_stop(string word, int to_ower = 1);
    void stem(string &word);
private:
    map<string, int> stop_list;
    stemmer stem_proc;
    void load_stop(const char *f);
};

#endif
