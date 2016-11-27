#ifndef _TOOLS
#define _TOOLS

#include <string>
#include <map>
#include <vector>
#include "stem.h"

using namespace std;
extern class tools *Global_Tools;

class tools {
public:
    tools(string stop_file, string query_stop, string file_txt);
    int is_stop(string word, int ignore_case = 1);
    string stem(string word);
    string get_trec_map(int docID);
private:
    map<string, int> stop_list;
    vector<string> trec_map;
    stemmer stem_proc;
    void load_stop(const char *f);
    void load_file(string file_txt);
};

#endif
